#include <myself.h>
int main() {
    int sockFd = socket(AF_INET,SOCK_STREAM,0);
    //创建一个socket 支持IPv4 和 TCP
    ERROR_CHECK(sockFd,-1,"socket");
    printf("sockFd = %d\n", sockFd);
    close(sockFd);
}