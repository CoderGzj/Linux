#include <myself.h>
int main(int argc, char *argv[]) {
    // ./client 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = sendto(sockFd,"hello udp",9,0,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"sendto");
    socklen_t socklen = sizeof(addr);
    char buf[4096] = {0};
    ret = recvfrom(sockFd,buf,sizeof(buf),0,(struct sockaddr *)&addr,&socklen);
    ERROR_CHECK(ret,-1,"recvfrom");
    puts(buf);
    close(sockFd);
}