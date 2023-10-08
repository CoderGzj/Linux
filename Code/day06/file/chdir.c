#include <myself.h>
int main(int argc, char*argv[]) {
    // ./chdir dir1
    ARGS_CHECK(argc,2);
    printf("before chdir, cwd = %s\n", getcwd(NULL,0));
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret,-1,"chdir");
    printf("after chdir, cwd = %s\n", getcwd(NULL,0));
}