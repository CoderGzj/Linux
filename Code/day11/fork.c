#include <myself.h>
int main(){
    pid_t pid = fork();
    if(pid == 0){
        printf("I am child, pid = %d, ppid = %d\n", getpid(),getppid());//操作系统不保证进程的执行先后顺序，不过通常进程创建大约在数百毫秒量级
    }
    else{
        printf("I am parent, pid = %d, ppid = %d\n", getpid(),getppid());
    //sleep(1); 如果不添加sleep，将会出现一些显示异常
    }
}