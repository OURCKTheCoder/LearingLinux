#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int main(int argc, char *argv[])
{
	// int open(char* path, int flags)
	// 成功返回文件描述符，失败返回-1
	int fd = open("TestFile", O_RDWR | O_CREAT | O_TRUNC);
	// flags的值在fcntl.h头文件中有定义
	// flags至少为：O_RDONLY 只读 O_WRONLY 只写 O_RDWR 读写
	// 额外的可以有：O_CREAT 新建 O_EXCL 创建不允许覆盖
	// O_TRUNC 可写打开时清空原有内容
	
    // size_t write(int fd, void *buf, size_t count) void *?
	char *buf = "This is some text for test.";
	if(fd != -1)
	{
		size_t size = write(fd, buf, strlen(buf));
		printf("Written: %s \n", buf);
	}
	else printf("Failed on opening file!");
	
	// int close(int fd) file descriptor may not be used at once. 
	close(fd); // 顺便重置指针位置

	// size_t read(int fd, void *buf, size_T COUNT)
	fd = open("TestFile", O_RDONLY);
	char *r_buf = malloc(strlen(buf));
	if(fd != -1)
	{
		size_t size;
		if((size = read(fd, r_buf, strlen(buf))) != -1)
			printf("Gotten %ld bytes: %s \n", size, r_buf);
		else
			printf("Reading file error!");
	}
	else printf("Failed on opening file!");
	free(r_buf);

	// off_t lseek(int fd, off_t offset, int whence)
	// int fstat(int fd, struct stat *buf) 
	struct stat statBuf;
	fstat(fd, &statBuf);
	// printf("fstat got: mode=%s, inode=%d, ctime=%d", statBuf.st_mode, statBuf.st_ino, statBuf.st_ctime); // How to print them out?

	// int stat(char*, stat buf) Get stat by name.
	// int lstat(char*, stat buf) Get stat by name. If a file is a link, get this link's detail instead of the file it refers to.
	// stdio lib:fopen:fclose fread fclose fflush ... (not concerning fd)(using buffer)

	// int main(int argc, char *argv[]) $exe 1 2 3. argc = 4.
	printf("This program received %d args \n", argc);
	int i = 0;
	for(i = 0; i < argc; i++)
	{
		printf("\t- arg%d = %s \n", i, argv[i]);
	}
	// int getopt(int argc, char *argv, char* optstring) Get the detail of args.
	extern char *optarg;
	extern int optind, opterr, optopt;
	char opt = 0;	
	while((opt = getopt(argc, argv, ":ab:c")) != -1)
	{
		if(opt == '?')
			printf("\t- Unknown option %c\n", optopt);
		else if(opt == ':') 
			printf("\t- Arg %c needs operand\n", optopt);
		else
			printf("\t- Received option %c, with %s\n",opt,optarg);
	}
	
	// int fork()
	// int pipe(int fd[2])


	return 0;
}
