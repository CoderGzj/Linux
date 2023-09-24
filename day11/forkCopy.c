#include <myself.h>
int global = 1;
int main() {
    int stack = 2;
    int *pHeap = (int *)malloc(sizeof(int));
    *pHeap = 3;
    if (fork() == 0) {
        printf("I am child process, global = %d, stack = %d, *pHeap = %d\n",
            global, stack, *pHeap);
        ++global;
        ++stack;
        ++*pHeap;
        printf("I am child process, global = %d, stack = %d, *pHeap = %d\n",
            global, stack, *pHeap);
    }
    else {
        sleep(3);
        printf("I am child process, global = %d, stack = %d, *pHeap = %d\n",
            global, stack, *pHeap);
    }
}