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
    //int ret = recv(sockFd,&dataLength,sizeof(int),MSG_WAITALL);
    int ret = recvn(sockFd,&dataLength,sizeof(int));
    ERROR_CHECK(ret,-1,"recv");
    //ret = recv(sockFd,name,dataLength,MSG_WAITALL);
    ret = recvn(sockFd,name,dataLength);
    ERROR_CHECK(ret,-1,"recv");
    int fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd,-1,"open");
    char buf[1000] = {0};
    while(1){ 
        //ret = recv(sockFd,&dataLength,sizeof(int),MSG_WAITALL);
        ret = recvn(sockFd,&dataLength,sizeof(int));
        ERROR_CHECK(ret,-1,"recv");
        if(dataLength != 1000){
            printf("dataLength = %d\n", dataLength);
        }
        if(dataLength == 0){
            break;
        }
        //ret = recv(sockFd,buf,dataLength,MSG_WAITALL);
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