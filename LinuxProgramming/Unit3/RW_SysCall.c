#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
// System call on file operations.
// This program includes:
/*	1. size_t write(int fd, const void *buf, size_t nbytes)
 *	2. size_t read(int fd, void *buf, site_t nbytes)
 *	3. int open(const char *path, int oflags, mode_t mode)
 *	   int open(const char *path, int oflags)
 *	4. int close(int fd)
 *	5. off_t lseek(int fd, off_t offset, int whence)
 *	6. int ioctl(int fd, int cmd, ...)
 */
int main()
{
	// 1. Open & create a file.
	int fd = open("TestFile", O_RDWR | O_CREAT | O_TRUNC);
	if(fd == -1) { printf("Error when opening file!"); return 1; }
	
	// 2. Write some cool shit in it.
	char buf[] = "This is some text for testing.";
	size_t written_bytes = write(fd, buf, strlen(buf));
	printf("%ld bytes written into file...\n", written_bytes);
	
	// Reset the fd ptr.
	lseek(fd, 0, SEEK_SET);
	
	// 3. Read cool shit out.
	const int MAX_BUF_SIZE = 50;
	char read_buf[MAX_BUF_SIZE];
	size_t read_bytes = read(fd, read_buf, MAX_BUF_SIZE);
	printf("%ld bytes read from file: %s\n", read_bytes, read_buf);	
	
	// 4. Close file.
	close(fd);
	
	return 0;
}
