#include <myself.h>
int main(int argc, char *argv[]) {
    // ./connect 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_STREAM,0);
    //创建一个socket 支持IPv4 和 TCP
    ERROR_CHECK(sockFd,-1,"socket");
    printf("sockFd = %d\n", sockFd);
    struct sockaddr_in addr; //创建用 sockaddr_in
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    //调用函数时，先取地址再强转
    int ret = connect(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    char buf[4096] = {0};
    ret = recv(sockFd,buf,sizeof(buf),0);
    ERROR_CHECK(ret,-1,"recv");
    puts(buf);
    ret = send(sockFd,"nihao from cilent",17,0);
    ERROR_CHECK(ret,-1,"send");
    close(sockFd);
}