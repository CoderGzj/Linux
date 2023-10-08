#include <myself.h>
int main(int argc, char *argv[]) {
    // ./chat1 1.pipe 2.pipe
    ARGS_CHECK(argc,3);
    int fdw = open(argv[1],O_WRONLY);
    int fdr = open(argv[2],O_RDONLY);
    puts("pipe open!");
    char buf[4096] = {0};
    fd_set rdset;
    while(1) {
        FD_ZERO(&rdset);
        FD_SET(fdr,&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        select(fdr+1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(fdr,&rdset)) {
            puts("msg from pipe");
            memset(buf,0,sizeof(buf));//清空
            int ret = read(fdr,buf,sizeof(buf));//从管道fdr读
            if (ret == 0) {
                printf("end!\n");
                break;
            }
            puts(buf);
        }
        if(FD_ISSET(STDIN_FILENO,&rdset)) {
            puts("msg from stdin");
            memset(buf,0,sizeof(buf));//清空
            int ret = read(STDIN_FILENO,buf,sizeof(buf));
            if (ret == 0) {
                write(fdw,"good man",8);
                break;
            }
            write(fdw,buf,strlen(buf));//向管道fdw写，有多少写多少
        }
    }
}