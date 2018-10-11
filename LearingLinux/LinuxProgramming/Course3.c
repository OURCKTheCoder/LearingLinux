#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

main()
{
	// pid_t fork() 子进程返回时得到0 父进程返回时得到子进程的pid
	// 错误返回-1
	// 子进程共享父进程的资源
	printf("Init!\n"); // Output only 1 time!
	printf("Init!"); // Output 2x times!
	// Linux与终端交互采用行缓冲，输出一行刷新一次缓冲区。
	// 如果带有换行符，父进程输出之后缓冲区被清空，
	// 子进程得到的缓冲区拷贝是空的。(？)
	// 实际上 子进程执行的代码也是从头开始的——与父进程一样。
	int pid;
	if((pid= fork() == 0)) {}
	else {}
	// fork()可以用在两种情况：1-父进程希望复制自己，去执行不同代码段；
	// 2-另一个进程执行不同程序。这时子进程返回时立即执行exec()，

	// void exit(int status)	
	// 终止进程：
	// 1-从main返回
	// 2-调用exit
	// 3-调用abort
	// 4-终止信号
	// 等

}
