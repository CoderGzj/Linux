#include <myself.h>
#define NUM 10

typedef struct queue_s{
    int elem[NUM];
    int size;
    int front;
    int rear;
}queue_t;

typedef struct shareRes_s{
    queue_t queue;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;

int cnt;

void *producer(void *arg){
    shareRes_t *pshareRes = (shareRes_t *)arg;
    while(1) {
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->queue.size == 10) {
            pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        }
        pshareRes->queue.elem[pshareRes->queue.rear] = cnt++;
        pshareRes->queue.rear = (pshareRes->queue.rear + 1) % NUM;
        ++pshareRes->queue.size;
        printf("producer size = %d, front = %d, rear = %d\n", 
            pshareRes->queue.size,
            pshareRes->queue.elem[pshareRes->queue.front],
            pshareRes->queue.elem[(pshareRes->queue.rear - 1 + NUM) % NUM]);
        if(pshareRes->queue.size == 1) {
            pthread_cond_signal(&pshareRes->cond);
        }
        pthread_mutex_unlock(&pshareRes->mutex);
        usleep(500000);
    }
}

void *consumer(void *arg){
    shareRes_t *pshareRes = (shareRes_t *)arg;
    while(1) {
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->queue.size == 0) {
            pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
        }
        pshareRes->queue.front = (pshareRes->queue.front + 1) % NUM;
        --pshareRes->queue.size;
        printf("consumer size = %d, front = %d, rear = %d\n", 
            pshareRes->queue.size,
            pshareRes->queue.elem[pshareRes->queue.front],
            pshareRes->queue.elem[(pshareRes->queue.rear - 1 + NUM) % NUM]);
        if(pshareRes->queue.size == 9) {
            pthread_cond_signal(&pshareRes->cond);
        }
        pthread_mutex_unlock(&pshareRes->mutex);
        sleep(1);
    }
}

int main() {
    shareRes_t shareRes;
    bzero(&shareRes.queue,sizeof(shareRes.queue));
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);

    pthread_t tid1,tid2;
    pthread_create(&tid1,NULL,producer,(void *)&shareRes);
    pthread_create(&tid2,NULL,consumer,(void *)&shareRes);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
}