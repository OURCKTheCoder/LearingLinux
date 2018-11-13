#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> // struct sockaddr_un
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HTTP_NONE		0x0000
#define HTTP_GET 		0x0001
#define HTTP_POST 	0x0010
#define HTTP_PUT 		0x0100
#define HTTP_DEL	 	0x1000

struct http_request_meta
{
	int method;
	char *req_path;
	char *http_ver;
};

const static int MAX_BUF_SIZE = 512;
const static char *SOCKET_NAME  = "server_socket";
static int server_skfd, client_skfd;

void InitServSocket();
int HandleResponseByRequestType(const char *request);
struct http_request_meta GetRequestMeta(const char *request);
char *GetRequestMetaline(const char *request);

int main(int argc, char *argv[])
{
	InitServSocket();
	while(1)
	{
		char ch;
		printf(" - Server waiting...\n");
		
		// 4. Accept a conn from client.
		struct sockaddr_un client_addr;
		size_t client_len = sizeof(client_addr); // TODO Or using socket_t.
		client_skfd = accept(server_skfd,
							 (struct sockaddr *)&client_addr,
							 (socklen_t *)&client_len);
		
		// Infomation exchanging.
		char buf[MAX_BUF_SIZE];
		size_t bytes = read(client_skfd, buf, MAX_BUF_SIZE);
		printf(" - Received msg(%ld bytes) from certain client:\n-------------------------------------\n%s\n-------------------------------------\n", bytes, buf);
		HandleResponseByRequestType(buf);
	}	
	return 0;
}

void InitServSocket()
{
	// 1. Create socket.
	unlink(SOCKET_NAME);
	server_skfd = socket(AF_UNIX, SOCK_STREAM, 0);
	
	// 2. Bind socket.
	struct sockaddr_un server_addr;
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_NAME);
	int res = bind(server_skfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	// TODO Success?
	
	// 3. Start listening & creating Q.
	listen(server_skfd, 10);
}

int HandleResponseByRequestType(const char *request)
{
	struct http_request_meta req_meta = GetRequestMeta(request);
	// TODO DEBUG-ONLY
	//printf("0x%x, %s, %s", req_meta.method, req_meta.req_path, req_meta.http_ver);
	
	// 3. Judge the HTTP method.
	char *response;
	switch(req_meta.method)
	{
	case HTTP_GET:		response = "You sent a GET, didn't you?"; break;
	case HTTP_POST:		response = "You sent a POST, didn't you?"; break;
	case HTTP_PUT:		response = "You sent a PUT, didn't you?"; break;
	case HTTP_DEL:		response = "You sent a DEL, didn't you?"; break;
	case HTTP_NONE:		response = "What the hell you sent to me?"; break;
	default: response = "If you see this msg, something strange happened..."; break;
	}
	
	// 4. Write response.
	write(client_skfd, response, strlen(response));
	
	return 0;
}

char *GetRequestMetaline(const char *request)
{
	char *metaline;
	int rear; // String's size including '\0'
	for(rear = 0; rear < strlen(request); rear++)
		if(request[rear] == '\n') break;
	rear++; // '\0'
	metaline = malloc(rear);
	int i;
	for(i = 0; i < rear; i++) metaline[i] = request[i];
	
	return metaline;
}

struct http_request_meta GetRequestMeta(const char *request)
{
	struct http_request_meta mapped;
	char *metaline = GetRequestMetaline(request);
	
	char *ptr;
	char *args[3];
	int counter = 0;
	ptr = strtok(metaline, " ");
	while (ptr != NULL && counter < 3) {
		//printf("ptr = %s\n", ptr); // TODO DEBUG-ONLY
		args[counter] = malloc(strlen(ptr));
		strcpy(args[counter], ptr);
		ptr = strtok(NULL, " ");
		counter++;
	}
	//int j;
	//for(j = 0; j < counter; j++) printf("%s", args[j]); // TODO DEBUG-ONLY
	
	int val;
	if(!strcmp(args[0], "GET")) 			val = HTTP_GET;
	else if(!strcmp(args[0], "POST")) 		val = HTTP_POST;
	else if(!strcmp(args[0], "PUT")) 		val = HTTP_PUT;
	else if(!strcmp(args[0], "DELETE"))		val = HTTP_DEL;
	else val = HTTP_NONE;
	
	mapped.method = val;
	mapped.req_path = malloc(strlen(args[1]));
	mapped.http_ver = malloc(strlen(args[2]));
	strcpy(mapped.req_path, args[1]);
	strcpy(mapped.http_ver, args[2]);
	
	return mapped;
}




