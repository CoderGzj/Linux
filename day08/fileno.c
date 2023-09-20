#include <myself.h>
int main (int argc, char *argv[]) {
    // ./fileno file1
    ARGS_CHECK(argc,2);
    FILE *fp = fopen(argv[1],"r+");
    ERROR_CHECK(fp,NULL,"fopen");
    printf("fileno = %d\n", fileno(fp));
    //int ret = write(3,"hello",5);//3是魔法数
    // int ret = write(fp->_fileno,"hello",5);//代码即注释
    int ret = write(fileno(fp),"hello",5);//面向接口编程
    ERROR_CHECK(ret,-1,"write");
    fclose(fp);
}