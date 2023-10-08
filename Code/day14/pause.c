#include<myself.h>
void sigFunc(int num) {
    printf("num = %d\n", num);
}
int main() {
    signal(SIGINT,sigFunc);
    pause();
}