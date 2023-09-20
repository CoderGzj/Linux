#include <myself.h>
int main(int argc, char *argv[]) {
    // ./cp src des
    ARGS_CHECK(argc,3);
    int fdr = open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    int fdw = open(argv[2],O_WRONLY | O_CREAT | O_TRUNC);
    ERROR_CHECK(fdw,-1,"open");
    char buf[4096] = {0};

    while(1) {
        //先清空
        memset(buf,0,sizeof(buf));
        ssize_t ret = read(fdr,buf,sizeof(buf));
        if (ret == 0) {
            break;
        }
        write(fdw,buf,ret);//第三个参数ret，表示读多少写多少。
    }
    close(fdr);
    close(fdw);
}