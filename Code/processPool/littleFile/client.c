#include <myself.h>
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;
int recvFile(int sockFd){
    char name[1024] = {0};
    int dataLength;
    int ret = recv(sockFd,&dataLength,sizeof(int),0);
    ERROR_CHECK(ret,-1,"recv");
    ret = recv(sockFd,name,dataLength,0);
    ERROR_CHECK(ret,-1,"recv");
    int fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd,-1,"open");
    char buf[1000] = {0};
    ret = recv(sockFd,&dataLength,sizeof(int),0);
    ERROR_CHECK(ret,-1,"recv");
    ret = recv(sockFd,buf,dataLength,0);
    ERROR_CHECK(ret,-1,"recv");
      write(fd,buf,ret);
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