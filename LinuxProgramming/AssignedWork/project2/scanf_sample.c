#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * P.S. 该程序仅用作练习，与本题目毫无关系！
 */
int main(int argc, char *argv[])
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
	printf("Get %d byte(s):\n%s", total, str);
	
	return 0;
}
