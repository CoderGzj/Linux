#include<myself.h>
int main() {
    int fds1[2];
    int fds2[2];
    pipe(fds1);
    pipe(fds2);
    if(fork() == 0) {
        close(fds1[1]);
        close(fds2[0]);
        write(fds2[1],"msg from child",14);
        char buf[1024] = {0};
        read(fds1[0],buf,sizeof(buf));
        puts(buf);
    }
    else {
        close(fds1[0]);
        close(fds2[1]);
        char buf[1024] = {0};
        read(fds2[0],buf,sizeof(buf));
        puts(buf);
        write(fds1[1],"msg from parent",15);
        wait(NULL);
    }
}