#include <myself.h>
void *threadFunc(void *) {
    printf("I am child thread, tid = %lu\n", pthread_self());
}
int main(){
    printf("I am main thread , tid = %lu\n", pthread_self());
    pthread_t tid;
    pthread_create(&tid, NULL, threadFunc, NULL);
    usleep(20);
    _exit(0);
    //return 0;
}