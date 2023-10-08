#include <myself.h>
void sigFunc(int num) {
    //printf("num = %d\n", num);
    //printf("num = %d", num); × 记得加上换行
    printf("before sleep, I  am %d\n", num);
    sleep(3);
    printf("after sleep, I am %d\n",num);
}

int main() {
    void (*ret)(int);
    ret = signal(SIGINT, sigFunc);
    ERROR_CHECK(ret,SIG_ERR, "signal");
    //ret = signal(SIGQUIT, sigFunc);
    //ERROR_CHECK(ret,SIG_ERR,"signal");
    while(1) {

    }
}