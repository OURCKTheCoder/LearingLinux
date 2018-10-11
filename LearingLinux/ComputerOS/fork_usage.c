#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>

void main()
{
	pid_t pid;
//	pid = fork();
	if(pid<0)
		printf("error!");
	else if(pid == 0)
		printf("I'm child process: #%d \n", getpid());
	else
		printf("I'm parent process: #%d \n", getpid());

}
