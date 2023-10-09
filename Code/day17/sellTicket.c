#include <myself.h>
typedef struct shareRes {
    int trainTicket;
    pthread_mutex_t mutex;
}shareRes_t;

void *sellTicket1 (void * arg) {
    shareRes_t *pShareRes = (shareRes_t *)arg;
    int cnt = 0;
    while(1) {
        pthread_mutex_lock(&pShareRes->mutex);
        if(pShareRes->trainTicket <= 0) {
            pthread_mutex_unlock(&pShareRes->mutex);
            break;
        }
        --pShareRes->trainTicket;
        ++cnt;
        pthread_mutex_unlock(&pShareRes->mutex);
    }
    printf("sellticket1 = %d\n", cnt);
}

void *sellTicket2 (void * arg) {
    shareRes_t *pShareRes = (shareRes_t *)arg;
    int cnt = 0;
    while(1) {
        pthread_mutex_lock(&pShareRes->mutex);
        if(pShareRes->trainTicket <= 0) {
            pthread_mutex_unlock(&pShareRes->mutex);
            break;
        }
        --pShareRes->trainTicket;
        ++cnt;
        pthread_mutex_unlock(&pShareRes->mutex);
    }
    printf("sellticket2 = %d\n", cnt);
}

int main() {
    pthread_t tid1,tid2;
    shareRes_t shareRes;
    shareRes.trainTicket = 20000000;
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_create(&tid1,NULL,sellTicket1,(void *)&shareRes);
    pthread_create(&tid2,NULL,sellTicket2,(void *)&shareRes);
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_mutex_destroy(&shareRes.mutex);
}