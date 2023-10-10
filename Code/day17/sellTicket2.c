#include <myself.h>
typedef struct shareRes_s {
    int flag;
    int trainTicket;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;

void *addTicket(void* arg) {
    shareRes_t *pshareRes = (shareRes_t *)arg;
    pthread_mutex_lock(&pshareRes->mutex);
    if(pshareRes->flag != 1) {
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }
    printf("add ticket!\n");
    pshareRes->trainTicket = 10;
    pthread_mutex_unlock(&pshareRes->mutex);
    pthread_exit(NULL);
}

void *sellTicket1(void* arg) {
    sleep(1);
    shareRes_t *pshareRes = (shareRes_t *)arg;
    while(1) {
        sleep(1);
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->trainTicket > 0) {
            printf("Before 1 sell, ticket = %d\n", pshareRes->trainTicket);
            --pshareRes->trainTicket;
            printf("after 1 sell, ticket = %d\n", pshareRes->trainTicket);
            if(pshareRes->trainTicket ==0 && pshareRes->flag ==0) {
                //若卖完票且从未放票
                pthread_cond_signal(&pshareRes->cond);
                pshareRes->flag == 1;
            }
            else if(pshareRes->trainTicket == 0 && pshareRes->flag != 0) {
                //若卖完票，且已经放票
                pthread_mutex_unlock(&pshareRes->mutex);
                break;
            }
        }
        else {
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pshareRes->mutex);
    }
    pthread_exit(NULL);
}

void *sellTicket2(void* arg) {
    sleep(1);
    shareRes_t *pshareRes = (shareRes_t *)arg;
    while(1) {
        sleep(1);
        pthread_mutex_lock(&pshareRes->mutex);
        if(pshareRes->trainTicket > 0) {
            printf("Before 2 sell, ticket = %d\n", pshareRes->trainTicket);
            --pshareRes->trainTicket;
            printf("after 2 sell, ticket = %d\n", pshareRes->trainTicket);
            if(pshareRes->trainTicket ==0 && pshareRes->flag ==0) {
                //若卖完票且从未放票
                pthread_cond_signal(&pshareRes->cond);
                pshareRes->flag == 1;
            }
            else if(pshareRes->trainTicket == 0 && pshareRes->flag != 0) {
                //若卖完票，且已经放票
                pthread_mutex_unlock(&pshareRes->mutex);
                break;
            }
        }
        else {
            pthread_mutex_unlock(&pshareRes->mutex);
            break;
        }
        pthread_mutex_unlock(&pshareRes->mutex);
    }
    pthread_exit(NULL);
}

int main() {
    shareRes_t shareRes;
    shareRes.flag = 0; //最开始不需要放票
    shareRes.trainTicket = 20; //初始票20
    pthread_mutex_init(&shareRes.mutex,NULL);
    pthread_cond_init(&shareRes.cond,NULL);

    pthread_t tid1, tid2, tid3;
    pthread_create(&tid1,NULL,sellTicket1,(void *)&shareRes);
    pthread_create(&tid2,NULL,sellTicket2,(void *)&shareRes);
    pthread_create(&tid3,NULL,addTicket,(void *)&shareRes);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    pthread_join(tid3,NULL);

    pthread_mutex_destroy(&shareRes.mutex);
    pthread_cond_destroy(&shareRes.cond);
}