#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<string.h>

int main(int argc, char *argv[])
{
	int in, out;
	char *ps, *pd;
	struct stat statbuf;
	if(argc < 2)
	{
		printf("Usage:%s srcfile desfile\n", argv[0]);
		exit(1);
	}
	in = open(argv[1], O_RDONLY);
	if(in < 0)
	{
		perror("open srcfile error");
		exit(1);
	}
	//if(fstat(in, &statbuf) == -1)
	if(stat(argv[1], &statbuf) == -1)
	{
		perror("fail to stat");
		exit(1);
	}
	printf("size : %d\n", (int)statbuf.st_size);
	ps = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, in, 0);
	if(ps == MAP_FAILED)
	{
		perror("mmap ps");
		exit(1);
	}
	close(in);

	out = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0644);
	if(out < 0)
	{
		perror("open desfile error");
		exit(1);
	}
#if 1	
	//对新建文件使用lseek撑大它的大小，要write任意一个字符
	//进去文件大小才会改变
	lseek(out, statbuf.st_size-1, SEEK_SET);
	write(out, "", 1);
#endif
	pd = mmap(NULL, statbuf.st_size, PROT_WRITE, MAP_SHARED, out, 0);
	if(pd == MAP_FAILED)
	{
		perror("mmap pd");
		exit(1);
	}
	close(out);
	memcpy(pd, ps, statbuf.st_size);
	munmap(ps, statbuf.st_size);
	munmap(pd, statbuf.st_size);

	return 0;
}

	
