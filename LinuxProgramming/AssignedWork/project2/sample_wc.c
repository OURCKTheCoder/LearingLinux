#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdbool.h>

#define BUF_SIZE 100

int bits_size = 5;
const char TERM_CHAR[2] = {' ', '\n'};

struct count_stat
{
	int char_num;
	int word_num;
	int line_num;
	char *name;
};

struct count_stat openFileAndCount(char *file);
struct count_stat strCount(char *str);
void inputAsStream();
void printResult(struct count_stat stat);

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

void printResult(struct count_stat stat)
{
	printf("%*d %*d %*d\t%s\n",
			bits_size, stat.line_num,
			bits_size, stat.word_num,
			bits_size, stat.char_num, stat.name);
}

/**
 * P.S. 统计结果可能与wc有出入，但基本与gedit的一致。
 * 		原因在于wc仅仅单纯地认为“以前后空格间隔开的一定是一个word“
 */
int main(int argc, char *argv[])
{
	//if(argc < 2)
	//{
	//	printf(" - Usage: wc [FILE] ...\n");
	//	return 0;
	//}

	switch(argc)
	{
		case 1:
			inputAsStream();
		default:
			for(int i = 1; i < argc; i++)
			{
				struct count_stat s = openFileAndCount(argv[i]);
				printResult(s);
			}
	}

	return 0;
} 

struct count_stat openFileAndCount(char *file)
{
	int char_counter = 0;
	int word_counter = 0;
	int line_counter = 0;

	int fd = open(file, O_RDONLY);
	// char *r_buf = malloc(BUF_SIZE); // TODO Bigger buffer?
	char r_buf;
	struct count_stat stat; // Counting result.
		
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
		
		stat.char_num = char_counter;
		stat.word_num = word_counter;
		stat.line_num = line_counter;
		stat.name = file;
	}
	else
		printf("Failed on opening file! \n");
		
	return stat;

}

struct count_stat strCount(char *str)
{
	int char_counter = 0;
	int word_counter = 0;
	int line_counter = 0;

	char r_buf;
	struct count_stat stat; // Counting result.
	
	if(str != NULL)
	{
		size_t total = 0;
		size_t size;
		bool onWordFlag = 0; // C90 has no BOOL !?
		for(char_counter = 0; char_counter < strlen(str); char_counter++)
		{
			char r_buf = str[char_counter];
			// printf("%c", r_buf); // TODO DEBUG-ONLY
			
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
		
		stat.char_num = char_counter;
		stat.word_num = word_counter;
		stat.line_num = line_counter;
		stat.name = malloc(1);
		
	}
	
	return stat;
}

void inputAsStream()
{
	char r_buf;
	int size = 0, total = 1;
	char *str = malloc(total); str[0] = '\0';
	while((size = scanf("%c", &r_buf)) >= 0)
	{
		total += size;
		char *old = str;
		str = malloc(total);
		strcpy(str, old);
		
		char cur[] = {r_buf, '\0'};
		strcat(str, cur);	
	}
	
	struct count_stat s = strCount(str);
	printResult(s);
	//printf("Get %d byte(s):\n%s", total, str); // TODO DEBUG-ONLY
}
