#include <myself.h>
typedef struct shareRes_s {
    int flag;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}shareRes_t;

void *threadFunc(void *arg) {
    shareRes_t *pshareRes = (shareRes_t *)arg;

    //先加锁
    pthread_mutex_lock(&pshareRes->mutex);
    //只有在加锁的状态下，才能使用wait
    if(pshareRes->flag == 0) {
        pthread_cond_wait(&pshareRes->cond,&pshareRes->mutex);
    }

    //从wait中唤醒，说明前面的事件已经完成了
    puts("world");
    pthread_mutex_unlock(&pshareRes->mutex);
}

int main() {
    //初始化锁和条件变量
    shareRes_t shareRes;
    pthread_mutex_init(&shareRes.mutex, NULL);
    pthread_cond_init(&shareRes.cond, NULL);
    shareRes.flag = 0;

    //创建一个子线程
    pthread_t tid;
    pthread_create(&tid,NULL,threadFunc,(void *)&shareRes);

    sleep(1);
    pthread_mutex_lock(&shareRes.mutex);

    //先执行一个事件，然后唤醒等待在条件变量上的某个线程
    puts("Hello");
    shareRes.flag = 1;
    pthread_cond_signal(&shareRes.cond);
    sleep(1);
    pthread_mutex_unlock(&shareRes.mutex);

    //收尾工作 等待子进程的终止，销毁锁和条件变量
    pthread_join(tid,NULL);
    pthread_cond_destroy(&shareRes.cond);
    pthread_mutex_destroy(&shareRes.mutex);
}