#include <myself.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc,3);
    int i1 = atoi(argv[1]);
    int i2 = atoi(argv[2]);
    printf("%d + %d = %d\n",i1,i2,i1+i2);
    printf("after exec! pid = %d\n",getpid());
}