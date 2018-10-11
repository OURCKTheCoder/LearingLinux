#include<unistd.h>
#include<stdlib.h>

extern char ** environ;

main()
{
	// putenv(const char *) 设置环境变量仅对当前程序有效
	// getenv(const char *) 获取环境变量
	// <stdlib.h> extern char ** environ 指向当前系统环境变量结构的指针
	char* val = getenv("HOME");
	printf("%s\n", val);
	printf("%s\n", *environ);
	
	// time_t time(time_t tloc) time_t与具体系统有关 该函数用于获取系统时间
	// struct tm *gmtime(const time_t *timeval) tm为一种数据结构
	// 返回格林尼治时间
	// void strftime(char *, int, char *, struct tm *) 获取格式化时间
	time_t ntime;
	ntime = time((time_t *)0);
	printf("%ld\n", ntime);

	// uid_t getuid()
	// char *getlogin() 获取用户名
	// struct passwd *getpwuid(uid_t) 获取用户密码(passwd结构体)
	// 当然不可能是明文
	uid_t uid = getuid();
	printf("%d, %s, %d \n", uid, getlogin(), getpwuid(uid));

	// int uname(struct utsname* name) 相当于Linux的uname 获得系统信息
	// void syslog(int priority, const char* msg) 写入系统日志

	// 资源管理
	// int getpriority(int which, id_t who) who可以是某个具体的进程号
	// 得到某个主体对某项占用资源的优先级？
	
	// 空间分配
	// void *malloc(size_t size) 不调用构造函数
	// void *calloc(size_t n, size_t size)
	// void free(void * ptr)


}
