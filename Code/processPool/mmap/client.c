#include <myself.h>
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;
int recvn(int sockFd,void *pstart,int len){
    int total = 0;
    int ret;
    char *p = (char *)pstart;
    while(total < len){
        ret = recv(sockFd,p+total,len-total,0);
        total += ret;
    }
    return 0;
}
int recvFile(int sockFd){
    char name[1024] = {0};
    int dataLength;
    int ret = recvn(sockFd,&dataLength,sizeof(int));
    ERROR_CHECK(ret,-1,"recv");
    ret = recvn(sockFd,name,dataLength);
    ERROR_CHECK(ret,-1,"recv");
    int fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd,-1,"open");
    int fileSize = 0;
    recvn(sockFd,&dataLength,sizeof(int));
    recvn(sockFd,&fileSize,dataLength);
    printf("fileSize = %d\n",fileSize);
    char buf[1000] = {0};
    int doneSize = 0;
    int lastSize = 0;
    int slice = fileSize/10000;
    while(1){
        ret = recvn(sockFd,&dataLength,sizeof(int));
        ERROR_CHECK(ret,-1,"recv");
        if(dataLength != 1000){
            printf("dataLength = %d\n", dataLength);
        }
        doneSize += dataLength;
        if(doneSize - lastSize > slice){
            printf("%5.2lf%%\r",100.0*doneSize/fileSize);
            fflush(stdout);
            lastSize = doneSize;
        }
        if(dataLength == 0){
            printf("100.00%%\n");
            break;
        }
        ret = recvn(sockFd,buf,dataLength);
        ERROR_CHECK(ret,-1,"recv");
        write(fd,buf,dataLength);
    }
}
int main(int argc, char *argv[]) {
    // ./cilent 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    recvFile(sockFd);
    close(sockFd);
}