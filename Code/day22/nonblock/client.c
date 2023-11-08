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
    //调用函数时，先去地址再强转
    //对于bind而言 addr一定是本地地址
    int ret = connect(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    sleep(10);
    send(sockFd,"hello",5,0);
    close(sockFd);
}