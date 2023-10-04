#include <myself.h>
void sigFunc(int num) {
    printf("before, num = %d\n", num);
    sleep(3);
    printf("after, num = %d\n", num);
}
void sigFunc3(int num, siginfo_t *siginfo, void *p) {
    printf("num = %d\n", num);
    printf("sender pid = %d\n", siginfo->si_pid);
}

int main() {
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler = sigFunc;
    //act.sa_sigaction = sigFunc3;
    act.sa_flags = SA_RESTART|SA_SIGINFO|SA_RESETHAND;
    //act.sa_flags = SA_RESTART|SA_NODEFER;
    int ret = sigaction(SIGINT,&act,NULL);
    ERROR_CHECK(ret,-1,"sigaction");
    char buf[100] = {0};
    read(STDIN_FILENO,buf,sizeof(buf));
    printf("buf = %s\n", buf);
}