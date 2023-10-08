#include <myself.h>
void sigFunc(int num) {
}
void mysleep(int second) {
    signal(SIGALRM,sigFunc);
    alarm(second);
    pause();
}
int main() {
    mysleep(5);
}