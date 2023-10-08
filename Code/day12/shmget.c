#include <myself.h>
int main() {
    key_t key = 1000;
    int shmid = shmget(1000,4096,IPC_CREAT|0600);
    ERROR_CHECK(shmid,-1,"shmget");
}