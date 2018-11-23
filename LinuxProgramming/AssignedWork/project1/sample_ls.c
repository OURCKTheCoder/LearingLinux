#include "recrusive_dir.h"
#include "dir_headers.h"

struct item_record
{
	char *str;
};

struct file_item
{
	int col_num;
	struct item_record *data;
	struct file_item *next;
};

struct item_table
{
	int num;
	struct file_item *head;
};

static bool flag_l, flag_R, flag_a, flag_i, flag_d;
const static size_t MAX_DIRCHAR_BUFSIZE = 100;

struct item_table genPwdTable(const char *path);
void printItemTable(struct item_table); 

 // TODO 应该写在ecrusive_dir.c里面 但如何编译？
// TODO Copy in depth?
struct str_list *catAsList(struct list_node *head)
{
	struct str_list *list = malloc(sizeof(struct str_list));
	if(head == NULL) return NULL;
	else
	{
		int iSize = 1;
		struct list_node *rear = head;
		while((rear = rear->next) != NULL) iSize++;
		list->size = iSize;
		list->head.next = head;
		return list;
	}

}

 // TODO 应该写在ecrusive_dir.c里面 但如何编译？
struct list_node *getAllChild(char *root)
{
	DIR *p_dir = opendir(root);
	struct dirent *child = NULL;
	struct list_node *child_list = NULL;
	struct list_node *list = NULL;
	struct list_node *rear = NULL;
	
	list = malloc(sizeof(struct list_node));
	list->name = malloc(strlen(root) * sizeof(char) + 1); // TODO !!!!!!!!!!!!!!!!!!!
	strcpy(list->name, root);
	list->next = NULL;
	rear = list;
	while(rear->next != NULL && (rear = rear->next) != NULL); // Short circuit.

	chdir(root);
	
	while((child = readdir(p_dir)) != NULL)
	{
		if(strcmp(child->d_name, ".") == 0 || strcmp(child->d_name, "..") == 0)
			continue;
		struct stat stat_buf;
		lstat(child->d_name, &stat_buf);
		if(!S_ISDIR(stat_buf.st_mode))
			continue;
		char cwd[MAX_BUF_LEN];
		getcwd(cwd, MAX_BUF_LEN);
		strcat(cwd, "/");
		strcat(cwd, child->d_name);
		child_list = getAllChild(cwd);

		rear->next = child_list;
		while(rear->next != NULL && (rear = rear->next) != NULL); // Short circuit.

	}
	
	chdir("..");
	return list;
}

int main(int argc, char *argv[])
{
	/* 1. Init flags */
/*	printf("This program received %d args \n", argc);*/
/*    int i = 0;*/
/*    for(i = 0; i < argc; i++)*/
/*    {*/
/*            printf("\t- arg%d = %s \n", i, argv[i]);*/
/*    }*/
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
				//printf("\t- Received option %c, with %s\n",opt,optarg);
				char c[] = {'-', opt, ' ', '\0'};
				strcat(final_args, c);
				switch(opt)
				{
					case 'l': flag_l = true; break;
					case 'R': flag_R = true; break;
					case 'a': flag_a = true; break;
					case 'i': flag_i = true; break;
					case 'd': flag_d = true; break;
					default: break;
				}
			}
    }
    
	int iDirs = 0;
	int i;
	for(; optind < argc; optind++) iDirs++; // argv[] changed.
	
	char **cmd_dirs = malloc(sizeof(char *) * iDirs);					// TODO DELETE
	for(i = 0; i < iDirs; i++)
	{
		cmd_dirs[i] = malloc(strlen(argv[argc - iDirs + i]) + 1);		// TODO DELETE
		strcpy(cmd_dirs[i], argv[argc - iDirs + i]);
	}
	
	// Switch -R
	struct list_node *target_dirs = malloc(sizeof(struct list_node));
	target_dirs->next = NULL;
	struct list_node *rear = target_dirs;
	if(flag_R)
	{
		int i;
		char ***head = malloc(sizeof(char **) * iDirs);					// TODO DELETE
		for(i = 0; i < iDirs; i++) head[i] = &cmd_dirs[i];
		
		// for each root, make a Q.
		for(i = 0; i < iDirs; i++)
		{
			struct list_node *child_lists = getAllChild(*head[i]);
			rear->next = child_lists;
			while(rear != NULL && rear->next != NULL) rear = rear->next;
		}
	}
	else
	{
		int i;
		for(i = 0; i < iDirs; i++)
		{
			struct list_node *node = malloc(sizeof(struct list_node));
			node->name = malloc(strlen(cmd_dirs[i]));
			strcpy(node->name, cmd_dirs[i]);
			rear->next = node;
			rear = rear->next;
		}
	}
	
	struct list_node *ptr = target_dirs->next;
	if(iDirs > 1)
	{
		while(ptr != NULL)
		{
			//printf("Direct argument: %s\n", argv[optind]);
			printf("%s:\n", ptr->name);
			struct item_table table = genPwdTable(ptr->name);
			printItemTable(table);
			
			ptr = ptr->next;
			printf("\n");
		}
	}
	else if(iDirs == 1)
	{
		struct item_table table = genPwdTable(ptr->name);
		printItemTable(table);
	}
	else // iDirs = 0
	{
		struct item_table table = genPwdTable(".");
		printItemTable(table);
	}
	
	return 0;
}

struct item_table genPwdTable(const char *path)
{
	char old_cwd[MAX_DIRCHAR_BUFSIZE];
	if(NULL == getcwd(old_cwd, MAX_DIRCHAR_BUFSIZE))
	{ printf(" [!] ERROR: Save old cwd failed!\n"); exit(1); }
	
	// 1. cd to current directory.
	if(chdir(path) != 0)
	{ printf(" [!] ERROR: chdir() failed! errno = %d\n - See http://man7.org/linux/man-pages/man3/errno.3.html\n", errno); exit(1); }
	
	char cwd[MAX_DIRCHAR_BUFSIZE];
	if(NULL == getcwd(cwd, MAX_DIRCHAR_BUFSIZE))
	{ printf(" [!] ERROR: MAX_DIRCHAR_BUFSIZE too small!\n"); exit(1); }

	// 2. open this dir.
	DIR *p_cd = NULL;
	if((p_cd = opendir(cwd)) == NULL)
	{ printf(" [!] ERROR: opendir() failed!\n"); exit(1); }
	
	// 3. scan current directory.
	int item_num = 0;
	struct dirent *dir_item = NULL;
	struct file_item *head = malloc(sizeof(struct file_item));			// TODO DELETE
	struct file_item *ptr = malloc(sizeof(struct file_item));				// TODO DELETE
	while((dir_item = readdir(p_cd)) != NULL)
	{
		char *abs_path = malloc(MAX_DIRCHAR_BUFSIZE);						 
		char *item_name = dir_item->d_name;
		// Used for concating pwd to the file name.
		if(!(strcmp(item_name, ".") == 0 || strcmp(item_name, "..") == 0))
		{
			strcpy(abs_path, cwd);
			strcat(abs_path, "/");
		}
		
		strcat(abs_path, item_name);

		// Always generate #COMPLETE# file info.
		const int record_num = 8;		
		struct file_item *item = malloc(sizeof(struct file_item));		// TODO DELETE
		struct item_record *records = malloc(sizeof(struct item_record) * record_num); //TODO DELETE
		struct stat *st = malloc(sizeof(struct stat));
		stat(abs_path, st);
		
		char *ino_str = malloc(sizeof(dir_item->d_ino)); 					// TODO DELETE
		sprintf(ino_str, "%ld", st->st_ino);
		records[0].str = ino_str;
		char *mode_str = malloc(sizeof(st->st_mode)); 						// TODO DELETE
		sprintf(mode_str, "%d", st->st_mode);
		records[1].str = mode_str;
		char *link_str = malloc(sizeof(st->st_nlink));						// TODO DELETE
		sprintf(link_str, "%ld", st->st_nlink);
		records[2].str = link_str;
		char *uid_str = malloc(sizeof(st->st_uid));						// TODO DELETE
		sprintf(uid_str, "%d", st->st_uid);
		records[3].str = uid_str;
		char *gid_str = malloc(sizeof(st->st_gid));						// TODO DELETE
		sprintf(gid_str, "%d", st->st_gid);
		records[4].str = gid_str;
		char *size_str = malloc(sizeof(st->st_size));						// TODO DELETE
		sprintf(size_str, "%ld", st->st_size);		
		records[5].str = size_str;
		char *ctime_str = malloc(sizeof(st->st_ctime));					// TODO DELETE
		sprintf(ctime_str, "%ld", st->st_ctime);
		records[6].str = ctime_str;
		records[7].str = malloc(strlen(item_name));							// TODO DELETE
		strcpy(records[7].str, item_name);
		
		item->col_num = record_num;
		item->data = malloc(sizeof(struct item_record) * record_num);		// TODO DELETE
		for(int i = 0; i < record_num; i++) item->data[i] = records[i];
		item->next = NULL;
		
		// Link to the head.
		ptr = head->next; 
		head->next = item;
		head->next->next = ptr;
		item_num++;
		
		free(abs_path);
	
	}
	
	struct item_table table;
	table.num = item_num;
	table.head = malloc(sizeof(struct file_item) * item_num);				// TODO DELETE
	
	struct file_item *p = head;
	int iItem = 0;
	while((p = p->next) != NULL)
	{
		table.head[iItem] = *p;
		iItem++;
	}	
	
	// X. CLOSE DIR; cd back to the old working dir.
	closedir(p_cd);
	chdir(old_cwd);
	
	return table;
}

void printItemTable(struct item_table table)
{
	struct file_item *p = table.head;
	while((p = p->next) != NULL) // TODO 少一个！！！！！！！！！！！！！！！！！！！！
	{		
		// Switch 1: -a
		if(p->data[7].str[0] == '.') 
			if(!flag_a) continue;
			
		// Switch 2: -i
		int iStart;
		if(!flag_i) iStart = 1;
		else		iStart = 0;
		for(int i = iStart; i < p->col_num; i++)
		{
			// Switch 3: -l
			if(!flag_l)
				if(i != 0 && i != 7) continue;
				
			printf("%s\t", p->data[i].str);
		}
		printf("\n");
	}	
}

