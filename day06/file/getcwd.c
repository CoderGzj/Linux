#include<myself.h>
int main() {
    // char buf[1024] = {0};
    // char *ret = getcwd(buf,sizeof(buf));
    // ERROR_CHECK(ret,NULL,"gecwd");
    // printf("ret = %p, ret = %s\n", ret, ret);
    // printf("buf = %p, buf = %s\n", buf, buf);
    printf("cwd = %s\n", getcwd(NULL,0));//不free注意内存泄漏
}