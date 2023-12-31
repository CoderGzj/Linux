#include <myself.h>
int sendFd(int pipeFd, int fdToSend, int exitFlag){
        struct msghdr hdr;
        bzero(&hdr,sizeof(hdr));
        // 传递文本
        struct iovec iov[1]; //长度为1时，离散数据退化为连续
        iov[0].iov_base = &exitFlag;
        iov[0].iov_len = sizeof(int);
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
int recvFd(int pipeFd, int *pFdToRecv, int *pExitFlag){
        struct msghdr hdr;
        bzero(&hdr,sizeof(hdr));
        // 传递文本
        struct iovec iov[1]; // 长度为1时，离散数据退化为连续
        iov[0].iov_base = pExitFlag;
        iov[0].iov_len = sizeof(int); //无论什么情况不要写0
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