#include <myself.h>
int main() {
    key_t key = ftok("file1",1);
    ERROR_CHECK(key,-1,"ftok");
    printf("key = %d\n", key);
}