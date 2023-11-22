#include "threadPool.h"
void cleanFunc(void *arg){
    threadPool_t *pThreadPool = (threadPool_t *)arg;
    pthread_mutex_unlock(&pThreadPool->taskQueue.mutex);
}
void * handleEvent(void *arg){
    threadPool_t * pThreadPool= (threadPool_t *)arg;
    int netFd;
    while(1){
        printf("I am free! tid = %lu\n", pthread_self());
        pthread_mutex_lock(&pThreadPool->taskQueue.mutex);
        pthread_cleanup_push(cleanFunc,(void *)pThreadPool);
        while(pThreadPool->taskQueue.size == 0){
            pthread_cond_wait(&pThreadPool->taskQueue.cond,&pThreadPool->taskQueue.mutex);   
        }
        // 子线程苏醒
        netFd = pThreadPool->taskQueue.pFront->netFd;
        taskDeQueue(&pThreadPool->taskQueue);
        //pthread_mutex_unlock(&pThreadPool->taskQueue.mutex);
        pthread_cleanup_pop(1);
        printf("I am working! tid = %lu\n", pthread_self());
        transFile(netFd);
        printf("done\n");
        close(netFd);
    }
}
int makeWorker(threadPool_t *pThreadPool){
    for(int i = 0; i < pThreadPool->threadNum; ++i){
        pthread_create(&pThreadPool->tid[i],NULL,handleEvent,(void *)pThreadPool);
    }
}