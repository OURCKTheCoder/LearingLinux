#include "recrusive_dir.h"
#include "dir_headers.h"

int main(int argc, char *argv[])
{
	struct list_node *list = getAllChild("/home/ourck/文档/Project_LinuxCHD/LearningLinux/LinuxProgramming/AssignedWork/project1");
	struct list_node *ptr = list;
	while(ptr != NULL)
	{
		printf("%s\n", ptr->name);
		ptr = ptr->next;
	}
	struct str_list *l = catAsList(list);
	printf("%d", l->size);
	
	return 0;
}

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
