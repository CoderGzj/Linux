#include <myself.h>
typedef struct mymesg {
    long mytype;
    char mtext[256];
}myMsg_t;

int main() {
    int msqid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msqid,-1,"msgget");
    myMsg_t msg1;
    myMsg_t msg2;
    myMsg_t msg3;
    msg1.mytype = 1;
    strcpy(msg1.mtext,"Hello Curry");
    msg2.mytype = 2;
    strcpy(msg2.mtext,"Hello klay");
    msg3.mytype = 3;
    strcpy(msg3.mtext,"Hello Green");
    msgsnd(msqid,&msg1,strlen(msg1.mtext),0);
    msgsnd(msqid,&msg2,strlen(msg2.mtext),0);
    msgsnd(msqid,&msg3,strlen(msg3.mtext),0);
    puts("send over");
}