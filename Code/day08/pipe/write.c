#include <myself.h>
int main(int argc,char *argv[]) {
    ARGS_CHECK(argc,2);
    int fdw = open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"OPEN");
    puts("pipe open!");
    write(fdw,"hello",5);
    close(fdw);
}