#include "head.h"
int transFile(int netFd){
    int fd = open("file1",O_RDWR);
    ERROR_CHECK(fd,-1,"open"); 
    train_t train;
    train.length = 5;
    strcpy(train.buf,"file1");
    int ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
    while(1){
        bzero(&train,sizeof(train));
        ret = read(fd,train.buf,sizeof(train.buf));
        ERROR_CHECK(ret,-1,"read");
        if(ret == 0){
            break;
        }
        train.length = ret;
        ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
        ERROR_CHECK(ret,-1,"send");
    }
    train.length = 0;
    ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    ERROR_CHECK(ret,-1,"send");
    close(fd);
}