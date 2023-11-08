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
    int netFd = accept(sockFd,NULL,NULL);
    ERROR_CHECK(netFd,-1,"accept");
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    struct epoll_event event;
    event.data.fd = STDERR_FILENO;
    event.events = EPOLLIN; 
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    event.data.fd = netFd;
    event.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,netFd,&event);
    char buf[4096] = {0};
    struct epoll_event readyArr[2];
    while(1) {
        int readyNum = epoll_wait(epfd,readyArr,2,-1);
        for(int i =0; i < readyNum; ++i) {
            if(readyArr[i].data.fd == STDERR_FILENO) {
                bzero(buf,sizeof(buf));
                int ret = read(STDERR_FILENO,buf,sizeof(buf));
                if(ret == 0) {
                    goto end;
                }
                send(netFd,buf,strlen(buf),0);
            }
            else if(readyArr[i].data.fd == netFd) {
                bzero(buf,sizeof(buf));
                int ret = recv(netFd,buf,sizeof(buf),0);
                if(ret == 0) {
                    goto end;
                }
                puts(buf);
            }
        }
    }
end:
    close(epfd);
    close(netFd);
    close(sockFd);
}