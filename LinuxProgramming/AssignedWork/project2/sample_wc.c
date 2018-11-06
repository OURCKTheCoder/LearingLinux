#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>

#define BUF_SIZE 100

const char TERM_CHAR[2] = {' ', '\n'};
void count(char *file);
int bits_size = 5;

int _gtbit(int val1, int val2) // TODO Calculate spaces for align.
{
	int c1 = 0, c2 = 0;
	while(val1 != 0 && c1++) val1 /= 10;
	while(val2 != 0 && c2++) val2 /= 10;
	//printf("c1 = %d, c2 = %d \n", c1, c2); // TODO DEBUG-ONLY
	bits_size = c1 < c2 ? c2 : c1;
	return bits_size;
}

bool isCharAChar(char c)
{
	return c >= 'A' && c <= 'Z' ||
			c >= '0' && c <= '9' ||
			c >= 'a' && c <= 'z';
}
/**
 * P.S. 统计结果可能与wc有出入，但基本与gedit的一致。
 */
int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf(" - Usage: wc [FILE] ...\n");
		return 0;
	}

	for(int i = 1; i < argc; i++)
	{
		count(argv[i]);
	}

	return 0;
} 

void count(char *file)
{
	int char_counter = 0;
	int word_counter = 0;
	int line_counter = 0;

	int fd = open(file, O_RDONLY);
	// char *r_buf = malloc(BUF_SIZE); // TODO Bigger buffer?
	char r_buf;
	
	if(fd != -1)
	{
		size_t total = 0;
		size_t size;
		bool onWordFlag = 0; // C90 has no BOOL !?
		while((size = read(fd, &r_buf, sizeof(r_buf))) != 0)
		{
			// printf("%c", r_buf); // TODO DEBUG-ONLY
			
			// 1. chars
			char_counter++;
			
			// 2. lines
			if(r_buf == '\n') line_counter++;
			
			// 3. words
			if(isCharAChar(r_buf))
			{
				onWordFlag = true;
			}
			if(onWordFlag && !isCharAChar(r_buf)) 
			{
				onWordFlag = false;
				word_counter++;
			}
			
			total += size;
		}
		
		printf("%*d %*d %*d\t%s\n", bits_size, line_counter, bits_size, word_counter, bits_size, char_counter, file);
		
	}
	else
		printf("Failed on opening file! \n");

}
