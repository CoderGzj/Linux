#include <myself.h>
int main (int argc, char *argv[]) {
    // ./mkdir dir1
    ARGS_CHECK(argc,2);
    int ret = mkdir(argv[1],0777);
    //所有创建文件的行为都会受umask影响
    ERROR_CHECK(ret,-1,"mkdir");
}