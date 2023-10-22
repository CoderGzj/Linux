#include <myself.h>
int main(int argc, char *argv[]) {
    // ./server 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockFd,-1,"socket");
    int optval = 1;
    int ret = setsockopt(sockFd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    ret = bind(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    ret = listen(sockFd,10);
    ERROR_CHECK(ret,-1,"listen");
    // accept要放在select之后
    // 去使用时确保从标准输入中输入数据在客户端建立连接后
    // accept之后创建新的netFd,这个netFd加入监听--->分离监听和就绪
    // 客户端如果断开连接以后，服务端不要退出，要取消监听netFd

    fd_set rdset; //单纯保存就绪的fd
    fd_set monitorSet;//使用一个单独的监听集合
    FD_ZERO(&monitorSet);
    FD_SET(sockFd,&monitorSet);
    char buf[4096] = {0};
    int netFdArr[10] = {0};
    int curConn = 0;
    while(1) {
        memcpy(&rdset,&monitorSet,sizeof(fd_set));
        select(20,&rdset,NULL,NULL,NULL);
        if(FD_ISSET(sockFd,&rdset)) {
            netFdArr[curConn] = accept(sockFd,NULL,NULL);
            ERROR_CHECK(netFdArr[curConn],-1,"accept");
            FD_SET(netFdArr[curConn],&monitorSet);
            printf("new connect is accepted!, curConn = %d\n", curConn);
            ++curConn;
        }
        for(int i = 0; i < curConn; ++i) {
            if(FD_ISSET(netFdArr[i],&rdset)) {
                bzero(buf,sizeof(buf));
                recv(netFdArr[i],buf,sizeof(buf),0);
                for(int j = 0; j < curConn; ++j) {
                    if(j == i) {
                        continue;
                    }
                    send(netFdArr[j],buf,strlen(buf),0);
                }
            }
        }
    }
    close(sockFd);
}