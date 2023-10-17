#include <myself.h>
int main(int argc, char *argv[]) {
    // ./ip 192.168.227.131
    ARGS_CHECK(argc,2);
    struct in_addr addr;
    inet_aton(argv[1],&addr);
    printf("addr = %08x\n", addr.s_addr);
} 