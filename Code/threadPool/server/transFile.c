#include "threadPool.h"
int transFile(int netFd){
    int fd = open("file1",O_RDWR);
    ERROR_CHECK(fd,-1,"open"); 
    train_t train;
    train.length = 5;
    strcpy(train.buf,"file1");
    int ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    struct stat statbuf;
    ret = fstat(fd,&statbuf);
    ERROR_CHECK(ret,-1,"fstat");
    train.length = 4; // 车厢是4个字节 int
    int fileSize = statbuf.st_size; // 长度转换成 int
    memcpy(train.buf,&fileSize,sizeof(int)); // int 存入小火车
    send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);

    sendfile(netFd,fd,NULL,fileSize);
    train.length = 0;
    ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    close(fd);
}