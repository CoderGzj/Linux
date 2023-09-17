#include <myself.h>
int main(int argc, char *argv[]) {
    // ./chmod 777 dir
    ARGS_CHECK(argc,3);
    // chmod(argv[2].argv[1])
    mode_t mode;
    sscanf(argv[1], "%o", &mode);
    int ret = chmod(argv[2],mode);
    ERROR_CHECK(ret,-1,"chmod");
}
