#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

/**
 * P.S. 默认的情况下，从管道中读写数据，最主要的特点就是阻塞问题。
 * 当管道里没有数据，另一个进程默认用 read() 函数从管道中读数据是阻塞的。
 * 调用 write() 函数向管道里写数据，当缓冲区已满时 write() 也会阻塞。
 */

const int P_SIZE = 3;
const int BUF_SIZE = 50;

void main()
{
	int fd[2]; // File Descriptors.
	char buf[BUF_SIZE];
	pipe(fd);
	
	int c_pid[P_SIZE];
	
	if((c_pid[0] = fork()) == 0)
	{
		// Child process sub start.
		// Output to buffer.
		sprintf(buf, "This is child process #%d. I fell cool!", getpid());
		write(fd[1], buf, BUF_SIZE);
		sleep(3); // working...
		exit(0);
	}
	else if((c_pid[1] = fork()) == 0)
	{
		// Child process sub start.
		// Output to buffer.
		sprintf(buf, "This is child process #%d. How are you?", getpid()); 
		write(fd[1], buf, BUF_SIZE);
		sleep(4); // working...
		exit(0);
	}
	else if((c_pid[2] = fork()) == 0)
	{
		// Child process sub start.
		// Output to buffer.
		sprintf(buf, "This is child process #%d. 23333333.", getpid()); 
		write(fd[1], buf, BUF_SIZE);
		sleep(2); // working...
		exit(0);
	}
	else
	{
		// Parent process on waiting.
		int count = 0;
		while(count++ < P_SIZE && wait(0)) // 0 as returned val of child process.
		{
			printf("Parent process #%d entered: certain child procecss exited & received msg from pipe! \n", getpid());
			char p_buf[BUF_SIZE];
			read(fd[0], p_buf, BUF_SIZE);
			printf("\t -> \" %s \" \n", p_buf);
		}
	}
}
