#include <myself.h>
#include "threadPool.h"
int main(int argc, char *argv[]){
    // ./server 192.168.227.131 1234 3
    int workerNum = atoi(argv[3]);
    threadPool_t threadPool; // 为线程池的任务队列、子线程的tid申请内存
    threadPoolInit(&threadPool,workerNum); // 初始化内存
    makeWorker(&threadPool); // 创建若干子线程
    int sockFd;
    tcpInit(&sockFd,argv[1],argv[2]); // 主线程要初始化tcp连接
    int epfd = epoll_create(1);
    epollAdd(sockFd,epfd); // 用epoll把sockFd监听起来
    struct epoll_event readyArr[2];
    while(1){
        int readyNum = epoll_wait(epfd,readyArr,2,-1);
        puts("epoll_wait returns");
        for(int i = 0; i < readyNum; ++i){
            if(readyArr[i].data.fd == sockFd){
                // 说明客户端有新的连接到来
                int netFd = accept(sockFd,NULL,NULL);
                // 先加锁
                pthread_mutex_lock(&threadPool.taskQueue.mutex);
                taskEnQueue(&threadPool.taskQueue,netFd); // 生产一个任务
                printf("New task!\n");
                pthread_cond_signal(&threadPool.taskQueue.cond); // 通知处于唤醒队列的子线程
                pthread_mutex_unlock(&threadPool.taskQueue.mutex);
            }
        }
    }
}
int threadPoolInit(threadPool_t *pThreadPool, int workerNum){
    pThreadPool->threadNum = workerNum;
    pThreadPool->tid = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
    pThreadPool->taskQueue.pFront = NULL;
    pThreadPool->taskQueue.pRear = NULL;
    pThreadPool->taskQueue.size = 0;
    pthread_mutex_init(&pThreadPool->taskQueue.mutex,NULL);
    pthread_cond_init(&pThreadPool->taskQueue.cond,NULL);
}
