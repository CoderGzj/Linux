# IPC 进程间通信
虚拟CPU和虚拟内存的引入保证了进程的一个重要特性就是隔离，一个进程在执行过程中总是认为自己占用了所有的CPU和内存

# **管道**
有名管道：在文件系统中存在一个管道文件
匿名管道：在文件系统中不存在 - 只用于父子进程间

## popen
FILE *popen(const char *command, const char *type);

"w" 父进程可写入FILE，子进程把自己的stdin重定向为管道；
"r" 父进程可读取FILE，子进程把自己的stdout重定向为管道

int pclose(FILE *stream);

![](img/2023-09-23-00-54-22.png)

## pipe 系统调用
使用系统调用 pipe 可以创建匿名管道，为了支持可移植性，管道是半双工的，所以一般同时使用两条管道来实现全双工通信。

除此以外，管道只使用于存在亲缘关系的进程之间进行通信。通常而言，在一个进程中可以创建一个管道，然后再利用fork 就可以实现进程间通信了。

int pipe(int pipefd[2]);

使用pipe 之前，需要首先创建一个大小为2的整型数组，用于存储文件描述符
pipefd[0]是读端，pipefd[1]是写端

如果要实现父子进程之间全双工通信，需要调用pipe 两次来创建两条管道

![](img/2023-09-23-16-14-07.png)

## FIFO
使用mkfifo 可以创建管道文件，
int mkfifo(const char *pathname, mode_t mode);

使用unlink 则可以删除所有文件包括管道文件。
int unlink(const char *path);

移动/重命名文件，使用rename 接口即可
int rename(const char *oldpath, const char *newpath);

使用link 接口可以给文件建立硬连接。
int link(const char *oldpath, const char *newpath);

# 共享内存
## 使用共享内存原因
* 为了提升进程间通信的效率，共享内存（也有翻译成共享存储）的方式就诞生了。
共享内存是效率最高的IPC

* 共享内存就允许两个或者多个进程共享一个给定的物理存储区域。当然为了实现共享，内核会专门维护一个用来存储共享内存信息的数据结构，这用不同的进程就可以通过共享内存进行通信了。
> $lsof
使用lsof命令可以列出所有进程所打开的文件
可以发现非常多进程都使用C标准库文件

## System V 版本的共享内存
* 内核使用一个非负整数键（key）来区分不同的共享内存区域（或者是信号量或消息队列）。服务端进程和客户端进程可以使用同一个键来定位共享内存段进行通信。

* 键可以手动指定，也可以使用接口ftok 生成。ftok 需要根据一个已存在的文件和一个项目ID（0～255的整数）来生成一个键。

key_t ftok(const char *pathname, int proj_id);

## 创建/获取共享内存
使用shmget 接口可以根据键来获取一个共享内存段。创建的共享内存段的所有字节会被初始化为0。

int shmget(key_t key, size_t size, int shmflg);

* key参数表示传入的键，可以是一个正数或者是宏IPC_PRIVATE。
* size表示共享内存的大小，其取值应当是页大小的整数倍。
* shmflg表示共享内存的属性，其最低9位表示各个用户对其的读/写/执行权限。
* shmget 的返回值表示共享内存段的描述符，以供后续使用。

使用shmat 接口根据一个指定描述来建立连接。

void *shmat(int shmid, const void *shmaddr, int shmflg);

shmaddr参数一般设置为空指针，表示在堆空间中自动分配区域映射共享内存段。shmflg表示权限（事实上这个权限是多余的），一般就是0。

![](img/2023-09-23-18-04-30.png)

## 使用共享内存进行进程间通信

## 两个进程同时对共享内存写入

## 解除共享内存映射

## 修改共享内存属性

## 私有共享内存

## 虚拟地址和物理地址转换机制

