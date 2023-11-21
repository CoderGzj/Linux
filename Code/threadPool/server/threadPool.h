#include <myself.h>
typedef struct task_s{
    int netFd; // 传递文件描述符
    struct task_s *pNext;
}task_t;
typedef struct taskQueue_s{
    task_t *pFront; // 队首指针
    task_t *pRear; // 队尾指针
    int size; // 队列现在的长度
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}taskQueue_t;
typedef struct threadPool_s{
    pthread_t *tid; // 子线程数组
    int threadNum; // 子线程数量
    taskQueue_t taskQueue; // 任务队列
}threadPool_t;
int taskEnQueue(taskQueue_t *pTaskQueue, int netFd);
int taskDeQueue(taskQueue_t *pTaskQueue);
int threadPoolInit(threadPool_t *pThreadPool, int workerNum);
int makeWorker(threadPool_t *pThreadPool);
int tcpInit(int *pSockFd, char *ip, char *port);
int epollAdd(int fd, int epfd);
int epollDel(int fd, int epfd);
int transFile(int netFd);
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;