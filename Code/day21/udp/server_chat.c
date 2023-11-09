#include <myself.h>
int main(int argc, char *argv[]) {
    // ./server_chat 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    char buf[4096] = {0};
    struct sockaddr_in clientaddr;   
    socklen_t clientLen = sizeof(clientaddr);
    ret = recvfrom(sockFd,buf,sizeof(buf),0,(struct sockaddr *)&clientaddr,&clientLen);
    ERROR_CHECK(ret,-1,"recvfrom");
    printf("buf = %s\n", buf);
    printf("client ip = %s\n", inet_ntoa(clientaddr.sin_addr));
    printf("client port = %d\n",ntohs(clientaddr.sin_port));
    fd_set rdset;
    while(1) {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sockFd,&rdset);
        select(sockFd + 1,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&rdset)) {
            bzero(buf,sizeof(buf));
            int ret = read(STDIN_FILENO,buf,sizeof(buf));
            if(ret == 0) {
                sendto(sockFd,buf,0,0,(struct sockaddr *)&clientaddr,sizeof(clientaddr));
                break;
            }
            sendto(sockFd,buf,strlen(buf),0,(struct sockaddr *)&clientaddr,sizeof(clientaddr));
        }
        if(FD_ISSET(sockFd,&rdset)) {
            bzero(buf,sizeof(buf));
            socklen_t addrLen = sizeof(addr);
            int ret = recvfrom(sockFd,buf,sizeof(buf),0,(struct sockaddr *)&clientaddr,&clientLen);
            if(ret == 0) {
                break;
            }
            puts(buf);
        }
    }
    close(sockFd);
}