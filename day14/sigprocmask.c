#include <myself.h>
void sigFunc(int num) {
    printf("num = %d\n", num);
}

int main() {
    signal(SIGQUIT,sigFunc);
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGQUIT);
    sigset_t oldset;
    sigprocmask(SIG_SETMASK,&set,&oldset);
    printf("I am going to sleep!\n");
    sleep(10);
    printf("wake up\n");
    sigprocmask(SIG_SETMASK,&oldset,NULL);
    printf("byebye!\n");
    return 0;
}