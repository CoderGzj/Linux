#include <myself.h>
int main() {
    pid_t pid = getpid();
    pid_t ppid = getppid();
    printf("pid = %d\n",pid);
    printf("ppid = %d\n",ppid);
}