#include <myself.h>
//int stat(const char *pathname, struct stat *statbuf);
int main(int argc, char *argv[]) {
    // ./myls dir
    ARGS_CHECK(argc,2);
    DIR * dirp = opendir(argv[1]);
    ERROR_CHECK(dirp,NULL,"opendir");
    struct dirent *pdirent;
    struct stat statbuf;
    int ret = chdir(argv[1]);
    ERROR_CHECK(ret,-1,"chdir");
    while((pdirent = readdir(dirp)) != NULL) {
        ret = stat(pdirent->d_name, &statbuf);
        ERROR_CHECK(ret,-1,"stst");
        char res;
        switch (statbuf.st_mode & S_IFMT) {
        case S_IFBLK:  
            res = 'b';
            break;
        case S_IFCHR:  
            res = 'c';        
            break;
        case S_IFDIR:  
            res = 'a';               
            break;
        case S_IFIFO:  
            res = 'p';               
            break;
        case S_IFLNK:  
            res = 'l';                 
            break;
        case S_IFREG:  
            res = '-';            
            break;
        case S_IFSOCK: 
            res = 's';                  
            break;
        default:       
            res = '?';                
            break;
        }

        printf("%c%6o %ld %s %s %8ld %d月 %d %2d:%2d %s\n",
        res,
        statbuf.st_mode,
        statbuf.st_nlink,
        getpwuid(statbuf.st_uid)->pw_name,
        getgrgid(statbuf.st_gid)->gr_name,
        statbuf.st_blksize,
        localtime(&statbuf.st_mtime)->tm_mon,
        localtime(&statbuf.st_mtime)->tm_mday,
        localtime(&statbuf.st_mtime)->tm_hour,
        localtime(&statbuf.st_mtime)->tm_min,
        pdirent->d_name);
    }

    closedir(dirp);
}