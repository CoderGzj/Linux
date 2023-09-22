#include <myself.h>
int main() {
    printf("gid = %d, egid = %d\n",getgid(),getegid());
    int fd = open("file1",O_WRONLY);
    ERROR_CHECK(fd,-1,"open");
    int ret = write(fd,"hello",5);
    ERROR_CHECK(ret,-1,"write");
}