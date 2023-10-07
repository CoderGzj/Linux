#include <myself.h>
#define NUM 10000000
void * threadFunc(void *arg) {
    int *pcount = (int *)arg;
    for(int i = 0; i < NUM; ++i) {
        ++*pcount;
    }
}
int main() {
    pthread_t tid;
    int count = 0;
    int ret = pthread_create(&tid,NULL,threadFunc, (void *)&count);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    for(int i = 0; i < NUM; ++i) {
        ++count;
    }
    pthread_join(tid,NULL);
    printf("count = %d\n", count);
}