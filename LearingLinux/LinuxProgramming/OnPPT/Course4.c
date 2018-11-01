#include<unistd.h>
#include<sys/types.h>

extern char ** environ

void main()
{
	// exec functions: P396
	// int execl(const char * path, const char * arg, ...)
	// int execve(const char *path, const char *argv[], const char* envp[]) 系统调用，各种exec最终的封装
	// exec会用新进程替换掉已有进程

	// interesting
}
