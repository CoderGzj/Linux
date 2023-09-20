#include <myself.h>
int main(int argc, char *argv[]) {
    ARGS_CHECK(argc,2);
    int oldfd = open(argv[1],O_RDWR);
    ERROR_CHECK(oldfd,-1,"open");
    printf("oldfd = %d\n", oldfd);
    int newfd = dup(oldfd);
    printf("newfd = %d\n", newfd);
    write(oldfd,"hello",5);
    close(oldfd);
    write(newfd,"world",5);
}