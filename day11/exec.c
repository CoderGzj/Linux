#include <myself.h>
int main() {
    printf("before exec! pid = %d\n",getpid());
    //execl("add","./add","3","4",NULL);
    char *argvector[] = {"./add","3","4",NULL};
    execv("add",argvector);
    printf("after exec!\n");
}