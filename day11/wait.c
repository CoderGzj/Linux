#include <myself.h>
int main () {
    pid_t pid;
    pid = fork();
    if(pid == 0) {
        printf("I am child, pid = %d, ppid = %d\n", getpid(), getppid());
        //return 1;
    }
    else {
        printf("I am parent, pid = %d, ppid = %d\n", getpid(), getppid());
        int wststus;
        wait(&wststus);
        if (WIFEXITED(wststus)) {
            printf("Normal exit! Return value = %d\n",WEXITSTATUS(wststus));
        }
        else if (WIFSIGNALED(wststus)) {
            printf("Abnormal exit! Terminal signal = %d\n", WTERMSIG(wststus));
        }
    }
}
