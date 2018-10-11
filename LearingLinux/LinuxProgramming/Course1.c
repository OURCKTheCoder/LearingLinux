#include<unistd.h>

main()
{
	// int open(char* path, int flags)
	// size_t read(int fd, void *nuf, size_T COUNT)
	// size_t write(int fd, void *buf, size_t count) void *?
	// int close(int fd) file descriptor may not be used at once. 
	// off_t lseek(int fd, off_t offset, int whence)
	// int fstat(int fd, struct stat *buf) 
	// int stat(char*, stat buf)
	// int lstat(char*, stat buf)
	// stdio lib:fopen:fclose fread fclose fflush ... (not concerning fd)(using buffer)

	// int main(int argc, char *argv[]) $exe 1 2 3. argc = 4.
	// int getopt(int argc, char *argv, char* optstring) Get the detail of args.
	
	// int fork()
	// int pipe(int fd[2])

}
