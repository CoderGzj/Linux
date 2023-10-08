#include <myself.h>
void mySystem(char *command);

int main() {
    //system(""./add 3 4");
    mySystem("./add 3 4");
}

void mySystem(char *command) {
    char buf[1024];
    strcpy(buf,command);
    char *argvec[10] = {0};
    char *word;
    word = strtok(buf," ");
    argvec[0] = word;
    //puts(word);
    int i = 1;
    while((word = strtok(NULL," ")) != NULL) {
        //puts(word);
        argvec[i++] = word;
    }
    if (fork() == 0) {
        execv(argvec[0],argvec);
    }
    else {
        wait(NULL);
    }
}