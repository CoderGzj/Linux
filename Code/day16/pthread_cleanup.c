#include <myself.h>
void clean1(void *arg) {
    printf("I am clean1\n");
    free(arg);
}
void clean2(void *arg) {
    printf("I am clean2\n");
    free(arg);
}
void * threadFunc(void *arg) {
    printf("I am child thread, tid = %lu\n", pthread_self());
    int *p1 = malloc(4);
    pthread_cleanup_push(clean1,p1);
    int *p2 = malloc(4);
    pthread_cleanup_push(clean2,p2);
    //return NULL;
    //pthread_cleanup_pop(1);
    //pthread_cleanup_pop(1);
    //sleep(1);
    int i;
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    //i = 1;
    pthread_exit(NULL);
}
int main() {
    printf("I am main thread, tid = %lu\n", pthread_self());
    pthread_t tid;
    int ret = pthread_create(&tid,NULL,threadFunc,NULL);
    THREAD_ERROR_CHECK(ret,"pthread_create");
}