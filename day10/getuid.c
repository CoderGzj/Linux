#include <myself.h>
int main() {
    uid_t uid = getuid();
    uid_t euid = geteuid();
    printf("uid = %d\n", uid);
    printf("euid = %d\n", euid);
}