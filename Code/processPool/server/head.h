#include <myself.h>
int tcpInit(int *pSockFd, char *ip, char *port);
int sendFd(int pipeFd, int fdToSend);
int recvFd(int pipeFd, int *pFdToRecv);
int epollAdd(int fd, int epfd);
int epollDel(int fd, int epfd);
int transFile(int netFd);
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;