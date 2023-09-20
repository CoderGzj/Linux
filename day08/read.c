#include <myself.h>
int main(int argc, char *argv[]) {
    // ./open file1
    ARGS_CHECK(argc,2);
    int fd = open(argv[1], O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("fd = %d\n",fd);
    char buf[10] = {0};
    ssize_t ret = read(fd,buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"read");
    puts(buf);
    close(fd);
}