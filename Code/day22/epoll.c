#include <myself.h>
int main() {
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd,-1,"epoll_create");
    printf("epfd = %d\n", epfd);
    close(epfd);
}