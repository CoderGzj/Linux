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

    int epfd = epoll_create(1);
    struct epoll_event event;
    event.data.fd = sockFd;
    event.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockFd,&event);
    char buf[4096] = {0};
    int netFdArr[10] = {0};
    int curConn = 0;
    struct epoll_event readyArr[11] = {0};
    while(1) {
        int readyNum = epoll_wait(epfd,readyArr,11,-1);
        puts("epoll_wait returns!");
        for(int i = 0; i < readyNum; ++i){
            if(readyArr[i].data.fd == sockFd){
                netFdArr[curConn] = accept(sockFd,NULL,NULL);
                ERROR_CHECK(netFdArr[curConn],-1,"accept");
                event.data.fd = netFdArr[curConn];
                event.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,netFdArr[curConn],&event);
                printf("new connect is accepted!, curConn = %d\n", curConn);
                ++curConn;
            }
            else{
                bzero(buf,sizeof(buf));
                recv(readyArr[i].data.fd,buf,sizeof(buf),0);
                for(int j = 0; j < curConn; ++j){
                    if(readyArr[i].data.fd == netFdArr[j]){
                        continue;
                    }
                    send(netFdArr[j],buf,strlen(buf),0);
                }
            }
        }
    }
    close(sockFd);
}