#include<myself.h>
int main() {
    int fds[2];
    pipe(fds);
    if(!fork()) {
        close(fds[1]);
        int fd = open("file2",O_RDWR|O_CREAT|O_TRUNC,0666);
        ERROR_CHECK(fd,-1,"open");
        char buf[4096] = {0};
        while(1) {
            bzero(buf,sizeof(buf));
            int ret = read(fds[0],buf,sizeof(buf));
            ERROR_CHECK(ret,-1,"read");
            if(ret == 0) {
                break;
            }
            write(fd,buf,ret);
        }
        close(fds[0]);
    }
    else {
        close(fds[0]);
        int fd = open("file1",O_RDWR);
        ERROR_CHECK(fd,-1,"open");
        char buf[4096];
        while(1) {
            bzero(buf,sizeof(buf));
            int ret = read(fd,buf,sizeof(buf));
            ERROR_CHECK(ret,-1,"read");
            if (ret == 0) {
                break;
            }
            write(fds[1],buf,ret);
        }
        close(fds[1]);
        wait(NULL);
    }
}