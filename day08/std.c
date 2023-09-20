#include <myself.h>
int main() {
    printf("fd of stdin = %d\n", fileno(stdin));
    printf("fd of stdout = %d\n", fileno(stdout));
    printf("fd of stderr = %d\n", fileno(stderr));
    //write(1,"hello",5);
    write(STDOUT_FILENO,"hello",5);
}