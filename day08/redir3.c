#include <myself.h>
int main (int argc, char *argv[]) {
    ARGS_CHECK(argc,2);
    int oldfd = open(argv[1],O_RDWR);
    ERROR_CHECK(oldfd,-1,"open");
    printf("You can see me!\n");
    int savefd = 5;
    dup2(STDOUT_FILENO,savefd);
    dup2(oldfd,STDOUT_FILENO);
    printf("You can't see me!\n");
    dup2(savefd,STDOUT_FILENO);
    printf("You can see me!\n");
}