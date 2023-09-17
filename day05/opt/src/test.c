#include <stdio.h>
#include "../include/test.h"
int main () {
#ifdef DEBUG
    puts("DEBUG exists!");
#endif
    printf("add(3,4) = %d", add(3, 4));
return 0;
}

int add(int l, int r) {
    return l + r;
}