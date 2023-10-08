#include <myself.h>
int main(int argc, char *argv[]) {
    // ./open file1
    ARGS_CHECK(argc,2);
    //int fd = open(argv[1], O_WRONLY);
    //int fd = open(argv[1],O_WRONLY|O_CREAT,0666);
    //创建文件行为，总是会受到umask影响
    int fd = open(argv[1],O_WRONLY|O_CREAT|O_EXCL,0666);
    ERROR_CHECK(fd,-1,"open");
    printf("fd = %d\n",fd);
    close(fd);
}