#include <myself.h>
int main() {
    FILE *fp = popen("./print","r");
    ERROR_CHECK(fp,NULL,"popen");
    char buf[1024] = {0};
    fread(buf,1,sizeof(buf),fp);
    printf("msg from pipe, buf = %s\n",buf);
}