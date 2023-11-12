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
        struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1,sizeof(struct cmsghdr));
        pcmsghdr->cmsg_len = CMSG_LEN(sizeof(int)); // 要承载的信息是一个整型数据
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
        struct cmsghdr *pcmsghdr = (struct cmsghdr *)calloc(1,sizeof(struct cmsghdr));
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
int main(){
    int sv[2];
    socketpair(AF_LOCAL,SOCK_STREAM,0,sv);
    if(fork() == 0){
        int fd = open("file",O_RDWR);
        printf("child fd = %d\n",fd);
        write(fd,"hello",5);
        sendFd(sv[1],fd);
        sleep(10);
        exit(0);
    }
    else{
        int fd = open("file1",O_RDWR);
        printf("parent fd = %d\n",fd);
        int newFd;
        recvFd(sv[0],&newFd);
        printf("parent newFd = %d\n",newFd);
        write(newFd,"world",5);
        wait(NULL);
    }
}