# 1 进程池和线程池的设计思路
一个良好的架构需要满足许多侧面的要求，其中最基本的要求是可维护性和性能

![](img/2023-11-09-21-03-38.png)

池化的思路
申请了资源之后，用完毕不要马上回收，可以交给另外的事情复用
进程池/线程池设计思路：
1. 提前创建好若干进程
2. 每当有任务到来分配一个进程
3. 任务完成后归还进程
4. 整个进程池关闭的时候再销毁

问题：
任务太多 -> 任务队列
事件 -> IO多路复用
事件驱动模型 event-driven

# 2 进程池的实现
![](img/2023-11-09-21-11-47.png)

## 2.1 父子进程创建
首先，我们先实现最基本的功能，使用一个父进程创建若干个子进程。

## 2.2 父进程处理网络连接
在创建完所有的子进程之后，父进程的下一个工作目标是准备接受客户端的TCP连接，这个工作和之前网络编程时的工作内容差不多，按照socket 、bind 和listen 的顺序执行系统调用即可。

## 2.3 本地套接字
父进程和子进程的地址空间是隔离的，如果两个进程之间需要进行通信，那就要选择一种合适的进程间通信的手段。
使用系统调用socketpair 可以在父子进程间利用socket 创建一个全双工的管道。除此以外，本地套接字可以在同一个操作系统的两个进程之间传递文件描述符。

int socketpair(int domain, int type, int protocol, int sv[2]);
* domain 必须填写AF_LOCAL。
* type 可以选择流式数据还是消息数据
* protocol 一般填0表示不需要任何额外的协议
* sv 这个参数和pipe 的参数一样，是一个长度为2的整型数据，用来存储管道两端的文件描述符（值得注意的是， sv[0] 和sv[1] 没有任何的区别）。

一般socketpair 之后会配合fork 函数一起使用，从而实现父子进程之间的通信。

## 2.4 父子进程共享文件描述符
除了传递一般的控制信息和文本信息（比如上传）以外，需要特别注意的是需要传递已连接套接字的文件描述符。
父进程会监听特定某个IP:PORT ，如果有某个客户端连接之后，子进程需要能够连上accept 得到的已连接套接字的文件描述符，这样子进程才能和客户端进行通信。这种文件描述符的传递不是简单地传输一个整型数字就行了，而是需要让父子进程共享一个套接字文件对象。
![](img/2023-11-09-21-32-07.png)

ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
使用sendmsg 和recvmsg 的时候附加一个消息头部，即一个struct msghdr 类型的结构体。
首先，需要将要传递的内容存储入msg_iov 当中，在这里需要注意的是，元素类型为struct iovec 的数组可以存储一组离散的消息，只需要将每个消息的起始地址和本消息的长度存入数组元素中即可。
接下来，需要将文件描述符的信息存入控制字段msg_control 中，这个我们需要存储一个地址值，该地址指向了一个struct cmsghdr 类型的控制信息。
为了传递文件描述符，需要将结构体中的cmsg_level 字段设置为SOL_SOCKET ，而 cmsg_type 字段需要设置为SCM_RIGHTS ，再将数据部分设置为文件描述符。这样，该文件描述符所指的文件对象就可以传递到另一个进程了。

![](img/2023-11-09-22-08-12.png)

![](img/2023-11-09-22-08-36.png)

总结：

![](img/2023-11-16-15-53-47.png)

# 3 文件的传输
文件传输的本质和cp 命令的原理一样：应用程序需要打开源文件并且进行读取，然后将读取得到的内容写入到目标文件当中。
如果是远程上传/下载文件，则需要将前述流程分解成两个应用程序，应用程序之间使用网络传输数据。

## 3.1 小文件传输和小火车
所谓的小文件，就是指单次send 和recv 就能发送/接收完成的文件。
要发送两个部分的数据：其一是文件名，用于对端创建文件；另一个部分是文件内容。

问题：TCP是一种字节流协议，消息无边界。
接收方无法区分文件名和文件内容。完全可能会出现把文件名和文件内容混杂在一起的情况，这种就是江湖中所谓的"粘包"问题。

接下要要做的是在应用层上构建一个私有协议，这个协议的目的是规定TCP发送和接收的实际长度从而确定单个消息的边界。

目前这个协议非常简单，可以把它看成是一个小火车，包括一个火车头和一个火车车厢。火车头里面存储一个整型数字，描述了火车车厢的长度，而火车车厢才是真正承载数据的部分。
```c
typedef struct train_s{
    int size;
    char buf[1000];
} train_t;
```
## 3.2 大文件传输
最自然的思路解决大文件问题就是使用循环机制：发送方使用一个循环来读取文件内容，每当读取一定字节的数据之后，将这些数据的大小和内容填充进小火车当中；接收方就不断的使用recv 接收小火车的火车头和车厢，先读取4个字节的火车头，再根据车厢长度接收后续内容。

一旦文件增大，会出现问题，而且还是不稳定出现问题：
* 最基本的问题发送的文件和接收到的文件大小不一致。
* 另外一个问题就是服务端可能会出现死循环。

![](img/2023-11-16-16-48-09.png)

![](img/2023-11-16-16-48-41.png)

## 3.3 忽略SIGPIPE信号
首先来解决死循环的问题，这个死循环的表现是服务端的epoll_wait 总是有文件描述符处于就绪状态，这个就绪的文件描述符就是工作进程的管道。
通过ps -elf 命令可以查看所有进程的状态，会发现该工作进程处于“僵尸”状态，“僵尸”状态出现的原因是该工作
进程终止但是父进程并未回收资源。
进入探查后，我们发现进程终止的原因是收到了信号SIGPIPE ，产生这个信号的原因是服务端往已经关闭的网络socket中写入数据了。
解决这个问题有两种方案，一种是使用signal 或者sigaction 忽略这个信号；另一种是给send 的最后一个参数加上MSG_NOSIGNAL 选项，这样进程也可以忽略信号。

```c
send(netFd,p,fileSize,MSG_NOSIGNAL);
```

## 3.4 让recv取出所有数据
另一个问题是传递数据和实际数据不一致的问题。
根源是调用recv 的时候，需要传入一个整型参数，但这个长度参数描述的是最大的长度，而实际recv 的长度可能并没有达到最大的长度。
因为TCP是一种流式协议，它只能负责每个报文可靠有序地发送和接收，但是并不能保证传输到网络缓冲区当中的就是完整的一个小火车。这样就有可能会到导致数据读取问题。
一种解决方案就是给recv 函数设置MSG_WAITALL 属性，这样的话， recv 在不遇到EOF或者异常关闭的情况就能一定把最大长度数据读取出来。

![](img/2023-11-16-17-11-19.png)

## 3.5 检查文件的正确性
判断这两个文件的内容是否是一致。最好的解决方式是使用md5码来解决问题。
md5 是一种摘要散列算法。它的行为类似于之前的哈希函数。如果两个文件的文件内容是一致的话，那么生成的md5码必然是一致的；如果两个文件的内容不同，只有极小的可以忽略不计的概率两个文件的md5码一致。
如果需要生成一个文件的md5码，需要使用命令md5sum。

>使用truncate命令可以生成指定大小的文件
>truncate -s 600M file2 

## 3.6 封装 recv MSG_WAITALL
为了简化recv的使用，可以考虑将之前带有MSG_WAITALL 参数的recv 封装成recvn
```c
int recvn(int netFd,void* pstart,int len){
    int total=0;
    int ret;
    char *p=(char*)pstart;
    while(total < len)
    {
    ret = recv(netFd,p+total,len-total,0);
    total += ret;//每次接收到的字节数加到total上
    }
    return 0;
}
```


# 4 进程池1.0
下面是一个能够正确下载任意大小文件的进程的代码实现：
## 4.1 客户端
```c
// client.c
#include <myself.h>
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;
int recvn(int sockFd,void *pstart,int len){
    int total = 0;
    int ret;
    char *p = (char *)pstart;
    while(total < len){
        ret = recv(sockFd,p+total,len-total,0);
        total += ret;
    }
    return 0;
}
int recvFile(int sockFd){
    char name[1024] = {0};
    int dataLength;
    int ret = recvn(sockFd,&dataLength,sizeof(int));
    ERROR_CHECK(ret,-1,"recv");
    ret = recvn(sockFd,name,dataLength);
    ERROR_CHECK(ret,-1,"recv");
    int fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd,-1,"open");
    char buf[1000] = {0};
    while(1){ 
        ret = recvn(sockFd,&dataLength,sizeof(int));
        ERROR_CHECK(ret,-1,"recv");
        if(dataLength != 1000){
            printf("dataLength = %d\n", dataLength);
        }
        if(dataLength == 0){
            break;
        }
        ret = recvn(sockFd,buf,dataLength);
        ERROR_CHECK(ret,-1,"recv");
        write(fd,buf,dataLength);
    }
}
int main(int argc, char *argv[]) {
    // ./cilent 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    recvFile(sockFd);
    close(sockFd);
}
```

## 4.2 服务端
```c
// main.c
#include "worker.h"
#include "head.h"
int main(int argc, char *argv[]){
    //./server 192.168.227.131 1234 3
    // 创建很多子进程
    int workerNum = atoi(argv[3]);
    workerData_t *workerArr = (workerData_t *)calloc(workerNum,sizeof(workerData_t));
    makeChild(workerArr,workerNum);

    // 初始化tcp连接
    int sockFd;
    tcpInit(&sockFd,argv[1],argv[2]);
    
    // 用epoll把tcp连接和子进程管理起来
    int epfd = epoll_create(1);
    epollAdd(sockFd,epfd);
    for(int i = 0; i < workerNum; ++i){
        epollAdd(workerArr[i].pipeFd,epfd);       
    }
    int listenSize = workerNum + 1;
    struct epoll_event *readyArr = (struct epoll_event *)calloc(listenSize,sizeof(struct epoll_event));
    while(1){
        int readyNum = epoll_wait(epfd,readyArr,listenSize,-1);
        printf("epoll_wait ready!\n");
        for(int i = 0; i < readyNum; ++i){
            if(readyArr[i].data.fd == sockFd){
                puts("client connect");
                int netFd = accept(sockFd,NULL,NULL);
                ERROR_CHECK(netFd,-1,"accept");
                for(int j = 0; j < workerNum; ++j){
                    if(workerArr[j].status == FREE){
                        printf("%d worker got a job, pid = %d",j,workerArr[j].pid);
                        sendFd(workerArr[j].pipeFd,netFd); // 把网络连接交给子进程
                        close(netFd);
                        workerArr[j].status = BUSY;
                        break;
                    }
                }
            }
            else{
                puts("one worker finishes his job!\n");
                for(int j = 0; j < workerNum; ++j){
                    if(readyArr[i].data.fd == workerArr[j].pipeFd){
                        pid_t pid;
                        int ret = read(workerArr[j].pipeFd,&pid,sizeof(pid));
                        printf("%d worker finish his job, pid = %d\n",j,pid);
                        workerArr[j].status = FREE;
                        break;
                    }
                }
            }
        }
    }
}

// worker.c
#include "worker.h"
#include "head.h"
int makeChild(workerData_t *workerArr, int workerNum){
    pid_t pid;
    int pipeFd[2];
    for(int i = 0; i < workerNum; ++i){
        socketpair(AF_LOCAL,SOCK_STREAM,0,pipeFd);
        pid = fork();
        if(pid == 0){
            //子进程 确保不会脱离if结构
            close(pipeFd[0]);
            eventHanler(pipeFd[1]);
        }
        //父进程
        close(pipeFd[1]);
        printf("pipeFd = %d, pid = %d\n",pipeFd[0],pid);
        workerArr[i].pipeFd = pipeFd[0];
        workerArr[i].pid = pid;
        workerArr[i].status = FREE;
    }
}
int eventHanler(int pipeFd){
    while(1){
        int netFd;
        recvFd(pipeFd,&netFd);
        // 后续任务加在这里
        printf("I got task!\n");
        transFile(netFd);
        printf("I have done this task!\n");
        pid_t pid = getpid();
        write(pipeFd,&pid,sizeof(pid));
        close(netFd);
    }
}

// tcp.c
// socket bind listen
#include "head.h"
int tcpInit(int *pSockFd, char *ip, char *port){
    *pSockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(*pSockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    int ret = setsockopt(*pSockFd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret = bind(*pSockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    listen(*pSockFd,10);
}

// epollFunc.c
#include "head.h"
int epollAdd(int fd, int epfd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}
int epollDel(int fd, int epfd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}

// sendFd.c
#include <myself.h>
int sendFd(int pipeFd, int fdToSend){
        struct msghdr hdr;
        bzero(&hdr,sizeof(hdr));
        // 传递文本
        struct iovec iov[1]; //长度为1时，离散数据退化为连续
        char buf[10] = "hello";
        iov[0].iov_base = buf;
        iov[0].iov_len = 5;
        hdr.msg_iov = iov;
        hdr.msg_iovlen = 1;
        // 传递控制信息
        struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1,CMSG_LEN(sizeof(int)));
        pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
        pcmsghdr->cmsg_level = SOL_SOCKET;
        pcmsghdr->cmsg_type = SCM_RIGHTS;
        *(int *)CMSG_DATA(pcmsghdr) = fdToSend;
        hdr.msg_control = pcmsghdr;
        hdr.msg_controllen = CMSG_LEN(sizeof(int));
        int ret = sendmsg(pipeFd,&hdr,0);
        ERROR_CHECK(ret,-1,"sendmsg");
        return 0;
}
int recvFd(int pipeFd, int *pFdToRecv){
        struct msghdr hdr;
        bzero(&hdr,sizeof(hdr));
        // 传递文本
        struct iovec iov[1]; // 长度为1时，离散数据退化为连续
        char buf[10] = {0};
        iov[0].iov_base = buf;
        iov[0].iov_len = sizeof(buf); //无论什么情况不要写0
        hdr.msg_iov = iov;
        hdr.msg_iovlen = 1;
        // 传递控制信息
        struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1,CMSG_LEN(sizeof(int)));
        pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
        pcmsghdr->cmsg_level = SOL_SOCKET;
        pcmsghdr->cmsg_type = SCM_RIGHTS;
        hdr.msg_control = pcmsghdr;
        hdr.msg_controllen = CMSG_LEN(sizeof(int));
        int ret = recvmsg(pipeFd,&hdr,0);
        ERROR_CHECK(ret,-1,"recvmsg");
        *pFdToRecv = *(int *)CMSG_DATA(pcmsghdr);
        return 0;
}

// transFile.c
#include "head.h"
int transFile(int netFd){
    int fd = open("file1",O_RDWR);
    ERROR_CHECK(fd,-1,"open"); 
    train_t train;
    train.length = 5;
    strcpy(train.buf,"file1");
    int ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    while(1){
        bzero(&train,sizeof(train));
        ret = read(fd,train.buf,sizeof(train.buf));
        if(ret == 0){
            break;
        }
        train.length = ret;
        ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    }
    train.length = 0;
    ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    close(fd);
}
```

# 5 进程池的其他功能
## 5.1 进度条显示
首先服务端需要传输一个文件的大小给客户端，以便客户端计算百分比。客户端也需要先接收一个长度的小火车，再读取文件内容，在显示的时候需要控制换行的显示，可以使用fflush 清空缓冲区。
```c
    // 服务端 发送文件大小
    struct stat statbuf;
    ret = fstat(fd,&statbuf);
    ERROR_CHECK(ret,-1,"fstat");
    train.length = 4; // 车厢是4个字节 int
    int fileSize = statbuf.st_size; // 长度转换成 int
    memcpy(train.buf,&fileSize,sizeof(int)); // int 存入小火车
    ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    
    // 客户端 进度条
    int fileSize = 0;
    recvn(sockFd,&dataLength,sizeof(int));
    recvn(sockFd,&fileSize,dataLength);
    printf("fileSize = %d\n",fileSize);
    char buf[1000] = {0};
    int doneSize = 0;
    int lastSize = 0;
    int slice = fileSize/10000;
    while(1){ 
        ret = recvn(sockFd,&dataLength,sizeof(int));
        ERROR_CHECK(ret,-1,"recv");
        if(dataLength != 1000){
            printf("dataLength = %d\n", dataLength);
        }
        doneSize += dataLength;
        if(doneSize - lastSize > slice){
            printf("%5.2lf%%\r",100.0*doneSize/fileSize);
            fflush(stdout);
            lastSize = doneSize;
        }
        if(dataLength == 0){
            printf("100.00%%\n");
            break;
        }
        ret = recvn(sockFd,buf,dataLength);
        ERROR_CHECK(ret,-1,"recv");
        write(fd,buf,dataLength);
    }
```

## 5.2 零拷贝、sendfile和splice
目前我们传输文件的时候是采用read 和send 来组合完成。
![](img/2023-11-16-18-24-54.png)
实际上这里涉及了大量的不必要的拷贝操作
如何减少从内核文件缓冲区到用户态空间的拷贝呢？解决方案就是使用mmap 系统调用直接建立文件和用户态空间buf的映射。这样的话数据就减少了一次拷贝。
![](img/2023-11-16-18-27-33.png)

```c
    // 服务端 分批发送
    char *p = (char *)mmap(NULL,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,MAP_FAILED,"mmap");
    int total = 0;
    while(total < fileSize){
        if(fileSize - total < sizeof(train.buf)){
            train.length = fileSize - total;
        }
        else{
            train.length = sizeof(train.buf);
        }
        memcpy(train.buf,p+total,train.length);
        send(netFd,&train,train.length + sizeof(train.length),MSG_NOSIGNAL);
        total += train.length;
    }
    train.length = 0;
    ret = send(netFd,&train,sizeof(train.length) + train.length,MSG_NOSIGNAL);
    close(fd);
    munmap(p,fileSize);
```
![](img/2023-11-16-18-32-37.png)

使用mmap只能减少数据从磁盘文件对象到用户态空间的拷贝，但是依然无法避免从用户态到内核已连接套接字的拷贝（因为网络设备文件对象不支持mmap ）。
sendfile 系统调用可以解决这个问题，它可以使数据直接在内核中传递而不需要经过用户态空间。
![](img/2023-11-18-23-52-31.png)
```c
#include <sys/sendfile.h>
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```
sendfile 只能用于发送文件方的零拷贝实现，无法用于接收方，并且发送文件的大小上限是2GB。

sendfile(netFd,fd,NULL,fileSize);

考虑到sendfile 只能将数据从磁盘文件发送到网络设备中，那么接收方如何在避免使用mmap 的情况下使用零拷贝技术呢？
一种方式就是采用管道配合splice 的做法。splice 系统调用可以直接将数据从内核管道文件缓冲区发送到另一个内核文件缓冲区。
![](img/2023-11-19-23-30-32.png)

## 5.3 进程池的退出
### 进程池的简单退出
父进程收到信号之后，再给每个子进程发送信号使其终止，要父进程在一个目标信号（通常是10信号SIGUSR1 ）的过程给目标子进程发送信号即可。
因为父进程会修改默认递送行为，而子进程会执行默认行为，所以fork 应该要在signal 的后面调用。

### 使用管道通知工作进程终止
采用信号就不可避免要使用全局变量。一种解决方案就是采取“异步拉起同步”的策略：虽然还是需要创建一个管道全局变量，但是该管道只用于处理进程池退出，不涉及其他的进程属性。
这个管道的读端需要使用IO多路复用机制管理起来，而当信号产生之后，主进程递送信号的时候会往管道中写入数据，此时可以依靠epoll 的就绪事件，在事件处理中来完成退出的逻辑。

### **进程池的优雅退出**
![](img/2023-11-22-16-36-23.png)

在之前的退出机制存在一个问题，就是即使工作进程正在传输文件中，父进程也会通过信号将其终止。如何实现进程池在退出的时候，子进程要完成传输文件的工作之后才能退出呢？
一种方案是使用sigprocmask 在文件传输的过程中设置信号屏蔽字
另一种方案就是调整sendFd 的设计，用户发送信号给父进程表明将要退出进程池；随后父进程通过sendFd给所有的工作进程发送终止的信息，工作进程在完成了一次工作任务了之后就会recvFd 收到进程池终止的信息，然后工作进程就可以主动退出；随着所有的工作进程终止，父进程亦随后终止，整个进程池就终止了。
```c
int sendFd(int pipeFd, int fdToSend, int exitFlag){
    struct msghdr hdr;
    bzero(&hdr,sizeof(struct msghdr));
    struct iovec iov[1];
    iov[0].iov_base = &exitFlag;
    iov[0].iov_len = sizeof(int);
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;
    //...
}
int recvFd(int pipeFd, int *pFd, int *exitFlag){
    struct msghdr hdr;
    bzero(&hdr,sizeof(struct msghdr));
    struct iovec iov[1];
    iov[0].iov_base = exitFlag;
    iov[0].iov_len = sizeof(int);
    hdr.msg_iov = iov;
    hdr.msg_iovlen = 1;
    //.....
}
void handleEvent(int pipeFd)
{
    int netFd;
    while(1){
        int exitFlag;
        recvFd(pipeFd,&netFd,&exitFlag);
        if(exitFlag == 1){
            puts("I am closing!");
            exit(0);
        }
        //...
    }
}
//... epoll
    for(int i = 0;i < readynum; ++i){
        if(readylist[i].data.fd == sockFd){
            puts("accept ready");
            int netFd = accept(sockFd,NULL,NULL);
            for(int j = 0;j < workerNum; ++j){
                if(workerList[j].status == FREE){
                    printf("No. %d worker gets his job, pid = %d\n", j,
                    workerList[j].pid);
                    sendFd(workerList[j].pipeFd, netFd, 0);
                    workerList[j].status = BUSY;
                    break;
                }
            }
            close(netFd);
        }
        else if(readylist[i].data.fd == exitpipeFd[0]){
            for(int j = 0; j < workerNum; ++j){
                puts("set exitFlag to worker!");
                sendFd(workerList[j].pipeFd,0,1);
            }
            for(int j = 0; j < workerNum; ++j){
                wait(NULL);
            }
            printf("Parent process exit!\n");
            exit(0);
        }
//....
```

# 6 线程池的实现
## 从进程池到线程池
使用进程池的思路来解决并发连接是一种经典的基于事件驱动模型的解决方案，但是进程天生具有隔离性，导致进程之间通信十分困难，一种优化的思路就是用线程来取代进程，即所谓的线程池。
由于多线程是共享地址空间的，所以主线程和工作线程天然地通过共享文件描述符数值的形式共享网络文件对象，但是这种共享也会带来麻烦：每当有客户端发起请求时，主线程会分配一个空闲的工作线程完成任务，而任务正是在多个线程之间共享的资源，所以需要采用一定的互斥和同步的机制来避免竞争。

可以将任务设计成一个队列，任务队列就成为多个线程同时访问的共享资源，此时问题就转化成了一个典型的生产者-消费者问题：任务队列中的任务就是商品，主线程是生产者，每当有连接到来的时候，就将一个任务放入任务队列，即生产商品，而各个工作线程就是消费者，每当队列中任务到来的时候，就负责取出任务并执行。

![](img/2023-11-22-21-29-04.png)

## 线程池的退出
### 简单退出
引入多进程机制：将进程池改造成一个父进程和一个子进程组成的应用程序。其中父进程负责递送信号，而子进程负责创建和运行进程池，父子进程之间通过管道通信。
当信号产生的时候，父进程递送该信号，并且在信号处理函数写入一个消息给管道，此外，子进程会使用IO多路复用机制监听管道，这样一旦管道就绪，子进程的主线程就可以得知程序将要被终止的信息，随后即可依次关闭子线程。

![](img/2023-11-23-00-07-00.png)

收到了取消后，线程会唤醒并终止，然而由于条件变量的设计，线程终止的时候它是持有锁的，这就导致死锁。这种死锁的解决方案就是引入资源清理机制，在加锁行为执行的时候立刻将清理行为压入资源清理栈当中。

### 优雅退出
如果使用pthread_cancel ，由于读写文件的函数是取消点，那么正在工作线程也会被终止，从而导致正在执行的下载任务无法完成。
不使用pthread_cancel ，而是让每个工作线程在事件循环开始的时候，检查一下线程池是否处于终止的状态，这样子线程就会等待当前任务执行完成了之后才会终止。

## 完整代码

```c
// threadPool.h
#include <myself.h>
typedef struct task_s{
    int netFd; // 传递文件描述符
    struct task_s *pNext;
}task_t;
typedef struct taskQueue_s{
    task_t *pFront; // 队首指针
    task_t *pRear; // 队尾指针
    int size; // 队列现在的长度
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}taskQueue_t;
typedef struct threadPool_s{
    pthread_t *tid; // 子线程数组
    int threadNum; // 子线程数量
    taskQueue_t taskQueue; // 任务队列
    int exitFlag;
}threadPool_t;
int taskEnQueue(taskQueue_t *pTaskQueue, int netFd);
int taskDeQueue(taskQueue_t *pTaskQueue);
int threadPoolInit(threadPool_t *pThreadPool, int workerNum);
int makeWorker(threadPool_t *pThreadPool);
int tcpInit(int *pSockFd, char *ip, char *port);
int epollAdd(int fd, int epfd);
int epollDel(int fd, int epfd);
int transFile(int netFd);
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;

// main.c
#include <myself.h>
#include "threadPool.h"
int exitPipe[2];
void sigFunc(int signum){
    printf("signum = %d\n", signum);
    write(exitPipe[1],"1",1);
    printf("Parent process is going to die!\n");
}
int main(int argc, char *argv[]){
    // ./server 192.168.227.131 1234 3
    ARGS_CHECK(argc,4);
    pipe (exitPipe);
    if(fork() != 0){ // 父进程 先注册一个信号，然后等待子进程终止，最后自己终止
        close(exitPipe[0]);
        signal(SIGUSR1,sigFunc);
        wait(NULL);
        exit(0);
    }
    close(exitPipe[1]);
    int workerNum = atoi(argv[3]);
    threadPool_t threadPool; // 为线程池的任务队列、子线程的tid申请内存
    threadPoolInit(&threadPool,workerNum); // 初始化内存
    makeWorker(&threadPool); // 创建若干子线程
    int sockFd;
    tcpInit(&sockFd,argv[1],argv[2]); // 主线程要初始化tcp连接
    int epfd = epoll_create(1);
    epollAdd(sockFd,epfd); // 用epoll把sockFd监听起来
    epollAdd(exitPipe[0],epfd);
    struct epoll_event readyArr[2];
    while(1){
        int readyNum = epoll_wait(epfd,readyArr,2,-1);
        puts("epoll_wait returns");
        for(int i = 0; i < readyNum; ++i){
            if(readyArr[i].data.fd == sockFd){
                // 说明客户端有新的连接到来
                int netFd = accept(sockFd,NULL,NULL);
                // 先加锁
                pthread_mutex_lock(&threadPool.taskQueue.mutex);
                taskEnQueue(&threadPool.taskQueue,netFd); // 生产一个任务
                printf("New task!\n");
                pthread_cond_signal(&threadPool.taskQueue.cond); // 通知处于唤醒队列的子线程
                pthread_mutex_unlock(&threadPool.taskQueue.mutex);
            }
            else if(readyArr[i].data.fd == exitPipe[0]){
                printf("child process, threadPool is going to die!\n");
                threadPool.exitFlag = 1;
                pthread_cond_broadcast(&threadPool.taskQueue.cond);
                for(int j = 0; j < workerNum; ++j){
                    pthread_join(threadPool.tid[j],NULL);
                }
                pthread_exit(NULL);
            }
        }
    }
}
int threadPoolInit(threadPool_t *pThreadPool, int workerNum){
    pThreadPool->threadNum = workerNum;
    pThreadPool->tid = (pthread_t *)calloc(workerNum,sizeof(pthread_t));
    pThreadPool->taskQueue.pFront = NULL;
    pThreadPool->taskQueue.pRear = NULL;
    pThreadPool->taskQueue.size = 0;
    pthread_mutex_init(&pThreadPool->taskQueue.mutex,NULL);
    pthread_cond_init(&pThreadPool->taskQueue.cond,NULL);
    pThreadPool->exitFlag = 0;
}

// worker.c
#include "threadPool.h"
void cleanFunc(void *arg){
    threadPool_t *pThreadPool = (threadPool_t *)arg;
    pthread_mutex_unlock(&pThreadPool->taskQueue.mutex);
}
void * handleEvent(void *arg){
    threadPool_t * pThreadPool= (threadPool_t *)arg;
    int netFd;
    while(1){
        printf("I am free! tid = %lu\n", pthread_self());
        pthread_mutex_lock(&pThreadPool->taskQueue.mutex);
        pthread_cleanup_push(cleanFunc,(void *)pThreadPool);
        while(pThreadPool->taskQueue.size == 0 && pThreadPool->exitFlag == 0){
            pthread_cond_wait(&pThreadPool->taskQueue.cond,&pThreadPool->taskQueue.mutex);   
        }
        if(pThreadPool->exitFlag != 0){
            printf("I am going to die child thread!\n");
            pthread_exit(NULL);
        }
        // 子线程苏醒
        netFd = pThreadPool->taskQueue.pFront->netFd;
        taskDeQueue(&pThreadPool->taskQueue);
        pthread_cleanup_pop(1);
        printf("I am working! tid = %lu\n", pthread_self());
        transFile(netFd);
        printf("done\n");
        close(netFd);
    }
}
int makeWorker(threadPool_t *pThreadPool){
    for(int i = 0; i < pThreadPool->threadNum; ++i){
        pthread_create(&pThreadPool->tid[i],NULL,handleEvent,(void *)pThreadPool);
    }
}

// epollFunc.c
#include "threadPool.h"
int epollAdd(int fd, int epfd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}
int epollDel(int fd, int epfd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}

// taskQueue.c
#include "threadPool.h"
int taskEnQueue(taskQueue_t *pTaskQueue, int netFd){ // 入队
    task_t *pTask = (task_t *)calloc(1,sizeof(task_t));
    pTask->netFd = netFd;
    if(pTaskQueue->size == 0){
        pTaskQueue->pFront = pTask;
        pTaskQueue->pRear = pTask;
    }
    else{
        pTaskQueue->pRear->pNext = pTask;
        pTaskQueue->pRear = pTask;
    }
    ++pTaskQueue->size;
    return 0;
}
int taskDeQueue(taskQueue_t *pTaskQueue){ // 出队
    task_t * pCur = pTaskQueue->pFront;
    pTaskQueue->pFront = pCur->pNext;
    free(pCur);
    pTaskQueue->size--;
    return 0;
}

// tcp.c
// socket bind listen
#include "threadPool.h"
int tcpInit(int *pSockFd, char *ip, char *port){
    *pSockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(*pSockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));
    addr.sin_addr.s_addr = inet_addr(ip);
    int reuse = 1;
    int ret = setsockopt(*pSockFd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ERROR_CHECK(ret,-1,"setsockopt");
    ret = bind(*pSockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"bind");
    listen(*pSockFd,10);
}

// transFile.c
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

// client.c
#include <myself.h>
typedef struct train_s{
    int length;
    char buf[1000];
}train_t;
int recvn(int sockFd,void *pstart,int len){
    int total = 0;
    int ret;
    char *p = (char *)pstart;
    while(total < len){
        ret = recv(sockFd,p+total,len-total,0);
        total += ret;
    }
    return 0;
}
int recvFile(int sockFd){
    char name[1024] = {0};
    int dataLength;
    int ret = recvn(sockFd,&dataLength,sizeof(int));
    ERROR_CHECK(ret,-1,"recv");
    ret = recvn(sockFd,name,dataLength);
    ERROR_CHECK(ret,-1,"recv");
    int fd = open(name,O_RDWR|O_CREAT|O_TRUNC,0666);
    ERROR_CHECK(fd,-1,"open");
    int fileSize = 0;
    recvn(sockFd,&dataLength,sizeof(int));
    recvn(sockFd,&fileSize,dataLength);
    printf("fileSize = %d\n",fileSize);
    char buf[1000] = {0};
    time_t timeBeg,timeEnd;
    timeBeg = time(NULL);

    int pipefds[2];
    pipe(pipefds);
    int total = 0;
    while(total < fileSize){
        int ret = splice(sockFd,NULL,pipefds[1],NULL,4096,SPLICE_F_MORE);
        total += ret;
        usleep(10000);
        splice(pipefds[0],NULL,fd,NULL,ret,SPLICE_F_MORE);
    }

    recvn(sockFd,&dataLength,sizeof(int));
    printf("dataLength = %d\n", dataLength);
    timeEnd = time(NULL);
    printf("total time = %ld\n",timeEnd - timeBeg);
}
int main(int argc, char *argv[]) {
    // ./cilent 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    int sockFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(sockFd,-1,"socket");
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = connect(sockFd,(struct sockaddr *)&addr,sizeof(addr));
    ERROR_CHECK(ret,-1,"connect");
    recvFile(sockFd);
    close(sockFd);
}
```