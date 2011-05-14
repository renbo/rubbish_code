#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc, const char *argv[])
{
    char *ps;
    char *pd;
    struct stat s;
    if (argc < 2) 
    {
        perror("Usage:%s srcfile desfile\n",argv[0]);
        exit(1);
    }
    int fd1 = open(argv[1],O_RDONLY);
    if (fd1 < 0) 
    {
        perror("open srcfile error");
        exit(1);
    }
    if (stat(argv[1],&s)==-1) 
    {
        perror("fail to stat");
        exit(1);
    }
    printf("size : %d\n",(int)s.st_size);
    ps = mmap(NULL,s.st_size,PROT_READ,MAP_PRIVATE,fd1,0);

    int fd2 = open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0644);
    if (fd2 < 0) 
    {
        perror("open destfile error");
        exit(1);
    }
    lseek(fd2,s.st_size-1,SEEK_SET);
    write(fd2,"",1);
    pd = mmap(NULL,s.st_size,PROT_WRITE,MAP_SHARED,fd2,0);
    if (pd == MAP_FAILED) 
    {
        perror("mmap pd");
        exit(1);
    }
    memcpy(pd,ps,s.st_size);
    close(fd1);
    close(fd2);
    munmap(ps,s.st_size);
    munmap(pd,s.st_size);
    return 0;
}
