#include <myself.h>

int main() {
    char sentence[] = "./add 3 4";
    char *word;
    word = strtok(sentence," ");
    printf("word = %s\n", word);
    word = strtok(NULL," ");
    printf("word = %s\n", word);
    word = strtok(NULL," ");
    printf("word = %s\n", word);
    word = strtok(NULL," ");
    printf("word = %s\n", word);
}