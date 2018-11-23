#define MAX_BUF_LEN	256

struct list_node
{
	char *name;
	struct list_node *next;
};

struct str_list
{
	int size;
	struct list_node head;
};

struct list_node *getAllChild(char *root);
struct str_list *catAsList(struct list_node *head);

