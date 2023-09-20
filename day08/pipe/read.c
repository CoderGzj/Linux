#include <myself.h>
int main(int argc,char *argv[]) {
    ARGS_CHECK(argc,2);
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"OPEN");
    puts("pipe open!");
    char buf[10] = {0};
    read(fdr,buf,sizeof(buf));
    puts(buf);
    close(fdr);
}