#include <myself.h>
int main (int argc, char *argv[]) {
    ARGS_CHECK(argc,2);
    printf("You can see me!\n");
    close(STDOUT_FILENO);
    int fd = open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("fd = %d\n",fd);
}