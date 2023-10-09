#include <myself.h>
void * threadFunc(void *arg) {
    printf("I am child thread, tid = %lu\n", pthread_self());
    int * pHeap = (int *)arg;
    *pHeap = 100;
    printf("child thread, *pHeap = %d\n", *pHeap);
}
int main() {
    printf("I am main thread, tid = %lu\n", pthread_self());
    int * pHeap = (int *)malloc(sizeof(int));
    *pHeap = 0;
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,(void *)pHeap);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    sleep(1);
    printf("main thread, *pHeap = %d\n", *pHeap);
}