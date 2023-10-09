#include <myself.h>
int global = 10;
void * threadFunc(void *arg) {
    printf("I am child thread, tid = %lu\n", pthread_self());
    printf("child thread, global = %d\n", global);
}
int main() {
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    sleep(1);
    global = 100;
    printf("main thread, global = %d\n", global);
}