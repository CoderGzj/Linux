#include<myself.h>
#define NUM 10000000

typedef struct shareRes_s {
    int count;
    pthread_mutex_t mutex;
}shareRes_t;

void *threadFunc(void *arg) {
    shareRes_t * shareRes = (shareRes_t *)arg;
    for (int i = 0; i < NUM; ++i) {
        pthread_mutex_lock(&shareRes->mutex);
        ++shareRes->count;
        pthread_mutex_unlock(&shareRes->mutex);
    }
}

int main() {
    shareRes_t shareRes;
    shareRes.count = 0;
    pthread_t tid;
    pthread_mutex_init(&shareRes.mutex,NULL);
    struct timeval timeBeg, timeEnd;
    gettimeofday(&timeBeg,NULL);
    int ret = pthread_create(&tid,NULL,threadFunc,(void *)&shareRes);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    for (int i = 0; i < NUM; ++i) {
        pthread_mutex_lock(&shareRes.mutex);
        shareRes.count++;
        pthread_mutex_unlock(&shareRes.mutex);
    }
    pthread_join(tid,NULL);
    gettimeofday(&timeEnd,NULL);
    printf("total time = %ld us\n",
        1000000*(timeEnd.tv_sec - timeBeg.tv_sec) + timeEnd.tv_usec - timeBeg.tv_usec);
    printf("count = %d\n", shareRes.count);
    pthread_mutex_destroy(&shareRes.mutex);
}