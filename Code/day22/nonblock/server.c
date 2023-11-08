#include <myself.h>

int setNonblock(int fd){
    int status = fcntl(fd,F_GETFL);
    status |= O_NONBLOCK;
    int ret = fcntl(fd,F_SETFL,status);
    ERROR_CHECK(ret,-1,"fcntl");
    return 0;
}

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
    int ret = bind(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    ret = listen(sockFd,10);
    ERROR_CHECK(ret,-1,"listen");
    struct sockaddr_in cilentAddr;
    socklen_t cilentAddrLen = sizeof(cilentAddr);
    int netFd = accept(sockFd,(struct sockaddr *)&cilentAddr,&cilentAddrLen);
    ERROR_CHECK(netFd,-1,"accept");
    printf("netFd = %d\n", netFd);
    printf("cilent addr length = %d\n", cilentAddrLen);
    printf("cilent family = %d\n", cilentAddr.sin_family);
    printf("cilent port = %d\n", ntohs(cilentAddr.sin_port));
    printf("cilent ip = %s\n", inet_ntoa(cilentAddr.sin_addr));
    char buf[4094] = {0};
    //setNonblock(netFd);

    ret = recv(netFd,buf,sizeof(buf),MSG_DONTWAIT);
    if(ret == -1) {
        puts("no resource now!\n");
    }
    ret = recv(netFd,buf,sizeof(buf),0);
    if(ret == -1) {
        puts("no resource now!\n");
    }
    //ERROR_CHECK(ret,-1,"recv");
    // while(1){
    //     ret = recv(netFd,buf,sizeof(buf),0);
    //     if(ret == -1){
    //         puts("no resource now!\n");
    //         sleep(1);
    //     }
    //     else{
    //         break;
    //     }
    // }
    puts(buf);
    close(sockFd);
}