#include <myself.h>
int main() {
    FILE *fp = popen("./sub","w");
    ERROR_CHECK(fp,NULL,"popen");
    fwrite("1000 999",1,8,fp);
}
