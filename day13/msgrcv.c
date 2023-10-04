#include <myself.h>
typedef struct mymesg {
    long mytype;
    char mtext[256];
}myMsg_t;

int main() {
    int msqid = msgget(1000,IPC_CREAT|0600);
    ERROR_CHECK(msqid,-1,"msgget");
    long type;
    printf("who are you? 1.curry 2.klay 3.green\n");
    scanf("%ld",&type);
    myMsg_t msg;
    memset(&msg,0,sizeof(msg));
    msgrcv(msqid,&msg,sizeof(msg.mtext),type,0);
    printf("you are %ld, msg = %s\n",type, msg.mtext);
}