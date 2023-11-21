#include "worker.h"
#include "head.h"
int workerNum;
workerData_t *workerArr;
void sigFunc(int signum){
    printf("signum = %d\n", signum);
    for(int i = 0; i < workerNum; ++i){
        kill(workerArr[i].pid,SIGUSR1);
    }
    for(int i = 0; i < workerNum; ++i){
        wait(NULL);
    }
    puts("process pool is over!");
    exit(0);
}
int main(int argc, char *argv[]){
    //./server 192.168.227.131 1234 3
    // 创建很多子进程
    workerNum = atoi(argv[3]);
    workerArr = (workerData_t *)calloc(workerNum,sizeof(workerData_t));
    makeChild(workerArr,workerNum);
    // 父进程要注册新号
    signal(SIGUSR1,sigFunc);
    // 初始化tcp连接
    int sockFd;
    tcpInit(&sockFd,argv[1],argv[2]);
    
    // 用epoll把tcp连接和子进程管理起来
    int epfd = epoll_create(1);
    epollAdd(sockFd,epfd);
    for(int i = 0; i < workerNum; ++i){
        epollAdd(workerArr[i].pipeFd,epfd);       
    }
    int listenSize = workerNum + 1;
    struct epoll_event *readyArr = (struct epoll_event *)calloc(listenSize,sizeof(struct epoll_event));
    while(1){
        int readyNum = epoll_wait(epfd,readyArr,listenSize,-1);
        printf("epoll_wait ready!\n");
        for(int i = 0; i < readyNum; ++i){
            if(readyArr[i].data.fd == sockFd){
                puts("client connect");
                int netFd = accept(sockFd,NULL,NULL);
                ERROR_CHECK(netFd,-1,"accept");
                for(int j = 0; j < workerNum; ++j){
                    if(workerArr[j].status == FREE){
                        printf("%d worker got a job, pid = %d",j,workerArr[j].pid);
                        sendFd(workerArr[j].pipeFd,netFd); // 把网络连接交给子进程
                        close(netFd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
            }
            else{
                puts("one worker finishes his job!\n");
                for(int j = 0; j < workerNum; ++j){
                    if(readyArr[i].data.fd == workerArr[j].pipeFd){
                        pid_t pid;
                        int ret = read(workerArr[j].pipeFd,&pid,sizeof(pid));
                        printf("%d worker finish his job, pid = %d\n",j,pid);
                        workerArr[j].status = FREE;
                        break;
                    }
                }
            }
        }
    }
}