#include <stdio.h>
void func(int i){
    printf("I am func , i = %d\n", i);
}
int main(){
    int j = 10;
    int arr[3] = {1,2,3};
    int *p;
    arr[2] = 4;
    p = arr;
    func(j);
    for(int i = 0;i < 5; ++i){
        puts("Hello world!");
    }
    return 0;
}

