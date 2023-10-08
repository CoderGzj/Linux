#include <myself.h>
int main(int argc,char *argv[]) {
    // ./ftruncate file1
    ARGS_CHECK(argc,2);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int ret = ftruncate(fd,30);
    ERROR_CHECK(ret,-1,"ftruncate");
}