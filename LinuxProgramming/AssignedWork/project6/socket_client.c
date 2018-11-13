#pragma comment(lib,"libiconv.lib")
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h> // struct sockaddr_un
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const static int MAX_BUF_SIZE = 512;
const static char *SERVER_SOCKET_NAME  = "server_socket";
static int target_skfd;

void InitClientSocket();

int main(int argc, char *argv[])
{
	InitClientSocket();
	
	// Infomation exchanging.
	char *msg = malloc(MAX_BUF_SIZE);
	char *str = "The quick brown fox jumps over the lazy dog.\n";
	char *pad = malloc(128);
	strcat(msg, "POST /home/ourck HTTP/1.1\n");
	strcat(msg, "Host: www.chd.edu.cn\n");
	strcat(msg, "Content-Type: application/x-www-form-urlencoded\n");
	strcat(msg, "Content-Length: ");
	strcat(msg, pad); // Padding.
	strcat(msg, "\n\n");
	strcat(msg, str); // Body
	strcat(msg, "\r\n\r\n");
	char buf[MAX_BUF_SIZE];
	
	write(target_skfd, msg, strlen(msg)); // TODO INSTEAD OF sizeof()!!!!!
	printf(" - Sent msg(%ld bytes) to certain server:\n------------------------------------- \n%s\n-------------------------------------\n", strlen(msg), msg);
	size_t bytes = read(target_skfd, buf, MAX_BUF_SIZE);
	printf(" - Received msg(%ld bytes) from certain server: ", bytes);
	/**
	 * TODO 行尾输出乱码问题？？？
	 * 通过套接字传输的文本（无论中英文）
	 * 很大概率在客户端输出没问题的文本，
	 * 在服务端输出会在行尾出现乱码，
	 * 反过来传输也一样。
	 *
	 * ——但若是直接按字节在本地再输出一遍，就不会再出现该问题。
	 * 这起码说明传输的数据是准确无误的。
	 * 那么这个问题是什么情况？真是因为采用UTF-8编码的网络传输所致？
	 *
	 * ——如果加上下面那个printf("\n");，也会出现该问题。 ->?
	 */
	int i;
	for(i = 0; i <= bytes; i++) printf("%c", buf[i]);
	printf("\n");
	
	return 0;
}

void InitClientSocket()
{
	// 1. Create socket: int socket(int domain, int type, int protocal)	
	target_skfd = socket(AF_UNIX, SOCK_STREAM, 0);

	// 2. Connect to server & name this socket.
	struct sockaddr_un terget_addr;
	terget_addr.sun_family = AF_UNIX;
	strcpy(terget_addr.sun_path, SERVER_SOCKET_NAME);
	int res = connect(target_skfd, (struct sockaddr *)&terget_addr, sizeof(terget_addr));
	// TODO Success?
}
