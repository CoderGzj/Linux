#include <myself.h>
void * threadFunc(void *arg) {
    printf("I am child thread, tid = %lu\n", pthread_self());
    //return (void *)0;
    pthread_exit((void *)0);
}
int main() {
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
    //long retval; //申请8个字节
    //pthread_join(tid, (void **)&retval); 
    //主线程要拿&retval所指的内存空间，存储子线程的返回值
    //printf("retval = %ld\n", retval);
    void *retval;
    pthread_join(tid,&retval);
    printf("retval = %ld\n", (long) retval);
}