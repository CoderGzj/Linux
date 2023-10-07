#include <myself.h>
void * threadFunc(void *arg) {
    while(1) {
        sleep(1);
    }
}
int main() {
    pthread_t tid;
    for(int i = 0;;++i) {
        int ret = pthread_create(&tid,NULL,threadFunc,NULL);
        THREAD_ERROR_CHECK(ret,"pthread_create");
        if(ret != 0) {
            printf("i = %d\n", i);
            break;
        }
    }
}