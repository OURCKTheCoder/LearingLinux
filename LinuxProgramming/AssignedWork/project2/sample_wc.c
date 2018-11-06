#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[])
{
	int fd = open("TestFile", O_RDONLY);
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
}
