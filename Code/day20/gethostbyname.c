#include <myself.h>
int main(int argc, char *argv[]) {
    // ./gethostbyname www.baidu.com
    ARGS_CHECK(argc,2);
    struct hostent *pHost = gethostbyname(argv[1]);
    if(pHost == NULL) {
        fprintf(stderr,"gethostbyname:%s\n", strerror(h_errno));
    }
    printf("Host real name = %s\n", pHost->h_name);
    for(int i = 0;pHost->h_aliases[i] != NULL; ++i) {
        printf("    alias name = %s\n", pHost->h_aliases[i]);
    }
    printf("addrtype = %d\n", pHost->h_addrtype);
    printf("addrlength = %d\n", pHost->h_length);
    for(int i = 0; pHost->h_addr_list[i] != NULL; ++i) {
        char buf[1024] = {0};
        inet_ntop(pHost->h_addrtype, pHost->h_addr_list[i], buf, sizeof(buf));
        printf("    addr = %s\n", buf);
    }
}