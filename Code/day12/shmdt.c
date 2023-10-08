#include <myself.h>
int main() {
    int shmid = shmget(1000,4096,0600|IPC_CREAT);
    ERROR_CHECK(shmid,-1,"shmget");
    char *p = (char *)shmat(shmid,NULL,0);
    ERROR_CHECK(p,(char *)-1,"shmat");
    int ret = shmdt(p);
    ERROR_CHECK(ret,-1,"shmdt");
    return 0;
}