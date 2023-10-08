#include<myself.h>
void sigFunc(int nim) {
    time_t now = time(NULL);
    puts(ctime(&now));
}
int main() {
    struct itimerval itimer;
    itimer.it_value.tv_sec = 5;
    itimer.it_value.tv_usec = 0;
    itimer.it_interval.tv_sec = 1;
    itimer.it_interval.tv_usec = 0;
    signal(SIGPROF, sigFunc);
    setitimer(ITIMER_PROF,&itimer,NULL);
    sigFunc(0);
    while(1);
}