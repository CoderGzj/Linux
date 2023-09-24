#include <myself.h>

void Daemon () {
    if (fork() != 0){
        exit(0);
    }
    setsid();
    for (int i = 0; i < 2; ++i) {
        close(i);
    }
    chdir("/");
    umask(0);
}

int main() {
    Daemon();
    for (int i = 0; i < 100; ++i) {
        time_t now = time(NULL);
        struct tm *ptm = localtime(&now);
        syslog (LOG_INFO,"%4d%2d%2d %2d:%2d:%2d\n",
            ptm->tm_year+1900,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
    sleep(2);
    }
}