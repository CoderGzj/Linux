#include <myself.h>
int main(int argc, char *argv[]) {
    // ./addr 192.168.227.131 1234
    ARGS_CHECK(argc,3);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    printf("port = %x\n", addr.sin_port);
    printf("ip = %08x\n", addr.sin_addr.s_addr);
}