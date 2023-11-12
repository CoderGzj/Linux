#include "worker.h"
int makeChild(workerData_t *workerArr, int workerNum){
    pid_t pid;
    int pipeFd[2];
    for(int i = 0; i < workerNum; ++i){
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipeFd);
        pid = fork();
        if(pid == 0){
            //子进程 确保不会脱离if结构
            close(pipeFd[0]);
            eventHanler(pipeFd[1]);
        }
        //父进程
        close(pipeFd[1]);
        printf("pipeFd = %d, pi = %d\n",pipeFd[0],pid);
        workerArr[i].pipeFd = pipeFd[0];
        workerArr[i].pid = pid;
        workerArr[i].status = FREE;
    }
}
int eventHanler(int pipeFd){
    while(1){
        int netFd;
        recvFd(pipeFd,&netFd);
        // 后续任务加在这里
        printf("I got task!\n");
        // done
        sleep(30);
        printf("I have done this task!\n");
        pid_t pid = getpid();
        write(pipeFd,&pid,sizeof(pid));
        close(netFd);
    }
}