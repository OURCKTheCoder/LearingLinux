#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <stdbool.h>

static bool flag_l, flag_R, flag_a, flag_i, flag_d;

int main(int argc, char *argv[])
{
	printf("This program received %d args \n", argc);
        int i = 0;
        for(i = 0; i < argc; i++)
        {
                printf("\t- arg%d = %s \n", i, argv[i]);
        }
        // int getopt(int argc, char *argv, char* optstring) Get the detail of args.
        extern char *optarg;
        extern int optind, opterr, optopt;
        char opt = 0;
		char final_args[50] = {0}; // TODO MAX_SIZE = 50
        while((opt = getopt(argc, argv, ":lRaid")) != -1)
        {
                if(opt == '?')
                        printf("\t- Unknown option %c\n", optopt);
                else if(opt == ':')
                        printf("\t- Arg %c needs operand\n", optopt);
                else
				{
					printf("\t- Received option %c, with %s\n",opt,optarg);
					char c[] = {'-', opt, ' ', '\0'};
					strcat(final_args, c);
				}
        }
	printf("----------------------------\n");
	printf("final_args = %s \n", final_args);
	char cmd[] = "ls ";
	strcat(cmd, final_args);
	
	char dir_args[100] = {'\0'}; // TODO MAX_SIZE = 100
	for(; optind < argc; optind++)
	{
		printf("Direct argument: %s\n", argv[optind]);
		strcat(dir_args, argv[optind]);
		char c[] = {' ', '\0'};
		strcat(dir_args, c);
	}
	strcat(cmd, dir_args);
	printf("final cmd = %s\n", cmd);	

	printf("---------------------------- \n");
	system(cmd);

	return 0;
}

