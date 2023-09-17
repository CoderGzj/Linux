#include <myself.h>
int main(int argc, char *argv[]) {
    // ./myls dir
    ARGS_CHECK(argc,2);
    DIR * dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *pdirent;
    long loc;
    long res;
    loc = telldir(dirp);
    for (; (pdirent = readdir(dirp)) != NULL;loc = telldir(dirp)) {
        if (strcmp(pdirent->d_name,"dir2") == 0) {
            res = loc;
        }
        printf("inode = %ld, reclen = %d, type = %d, name = %s\n",
            pdirent->d_ino, pdirent->d_reclen, pdirent->d_type, pdirent->d_name);
    }
    puts("---------------------------------------");
    rewinddir(dirp);
    //seekdir(dirp,res);
    pdirent = readdir(dirp);
    printf("inode = %ld, reclen = %d, type = %d, name = %s\n",
        pdirent->d_ino, pdirent->d_reclen, pdirent->d_type, pdirent->d_name);
    closedir(dirp);
}