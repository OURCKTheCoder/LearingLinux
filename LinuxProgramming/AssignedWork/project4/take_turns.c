#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sem.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/**
 * P.S. Ubuntu 18.04中的sem.h路径：
 * 			usr/include/x86_64-linux-gnu/sys/sem.h
 * 		查看后发现根本没有semun的声明，得自己动手。
 */
union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

const key_t MUTEX_PROTOCAL = 0x233; // Semp id shared between progeams.
const key_t TOKEN_FOR_PARENT_PROTOCAL = 0x777;
const key_t TOKEN_FOR_CHILD_PROTOCAL = 0x888;
int MUTEX_ID;
int P_TOKEN_ID;
int C_TOKEN_ID;
const char *TESTFILE_NAME = "TestFile";

int counter = 0;

int Initialize();
int getAndIncrement();
/**
 * TODO 在保证了互斥的基础上，还得保证交替。
 * 但由于可能Linus自己都不知道Linux是怎么 #实时# 调度进程的，
 * 例如OS有可能用两个连续的处理机来调度执行同一个进程。
 * 这样道理上应该就达不到“交替”的设计目标。
 *
 * 一种可能的情形是：
 * 第一个处理机周期中，子进程执行；
 * 第二个处理机周期中，父进程还没来得及上锁，OS就调度入子进程执行了。
 * 以上的情况是可以被观测到的，可以通过在P()原语操作之前让该进程小睡一会来观察。
 * 另外，实际上P()操作应该是原子性的。
 * 
 * 但注意：用标志量（“令牌”）来做自旋的套路是不可行的。
 * 因为fork()会令子进程得到父进程的一些运行时数据的拷贝，
 * 这就导致对标志量的修改只会在该进程（父进程 OR 子进程）自己的内存空间生效。
 * ——但为什么counter不会出现这个问题？
 * 
 * 不知道除了在设置一个信号量之外，还有什么更优雅的做法？
 */
//static int tokenForChild = 1;
static int P(int semp_id);
static int V(int semp_id);
static int del_semp(int semp_id);

static int del_semp(int semp_id)
{
	union semun buf;
	
	if(semctl(MUTEX_ID, 0, IPC_RMID) == -1)
	{
		fprintf(stderr, "Failed to delete semp id = %d! \n", semp_id);
		exit(1);
	}
} 

static int P(int semp_id)
{
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;
	
	if(semop(semp_id, &buf, 1) == -1)
	{
		fprintf(stderr, "Failed to P()! \n");
		del_semp(semp_id);
		exit(1);
	}
	
	//printf("LOCKED by pid = %d \n", getpid());
	return 1;
}

static int V(int semp_id)
{
	struct sembuf buf;
	buf.sem_num = 0;
	buf.sem_op = 1;
	buf.sem_flg = SEM_UNDO;
	
	if(semop(semp_id, &buf, 1) == -1)
	{
		fprintf(stderr, "Failed to V()! \n");
		del_semp(semp_id);
		exit(1);
	}
	
	//printf("RELEASED by pid = %d \n", getpid());
	return 1;
}

int main(int argc, char *argv[])
{
	if(Initialize() == -1) { printf("Error when initializing!\n"); return 1; }

	pid_t pid = fork();
	if(pid == -1) { printf("Error when forking!\n"); return 1; }
	
	while(1)
	{
		switch(pid)
		{
			case 0:
				P(C_TOKEN_ID);
				P(MUTEX_ID);
				printf("  o\t - Child (pid = %d) is ready to access file.\n", getpid());
				getAndIncrement();
				sleep(1);
				V(MUTEX_ID);
				V(P_TOKEN_ID);
				break;
				
			default:
				P(P_TOKEN_ID);
				P(MUTEX_ID);
				printf("o\t - Parent (pid = %d) is ready to access file.\n", getpid());
				getAndIncrement();
				// TODO 建议让子进程不sleep去入侵父进程上锁的资源区，来测试程序健壮性
				//sleep(1);
				V(MUTEX_ID);
				V(C_TOKEN_ID);
				break;
		}
	}
	return 0;
}

int Initialize()
{
	// 1. Open & create a file.
	int fd = open(TESTFILE_NAME, O_RDWR | O_CREAT | O_TRUNC);
	if(fd == -1) { printf("Error when opening file!\n"); return -1; }
	
	// 2. Write some cool shit in it.
	int size = counter / 10 + 1; 	// Calculate bytes' size.
	char str[size + 1];				// + '\0'
	sprintf(str, "%d", counter++);
	write(fd, str, strlen(str));
	
	// Reset the fd ptr.(Ignore)
	//lseek(fd, 0, SEEK_SET);
	
	// 3. Read cool shit out.(Ignore)
	
	// 4. Close file.
	close(fd);
	
	// 5. TODO Initialize semp.
	MUTEX_ID = semget(MUTEX_PROTOCAL, 1, IPC_CREAT | 0666);
	P_TOKEN_ID = semget(TOKEN_FOR_PARENT_PROTOCAL, 1, IPC_CREAT | 0666);
	C_TOKEN_ID = semget(TOKEN_FOR_CHILD_PROTOCAL, 1, IPC_CREAT | 0666);
	
	union semun orig_val_1, orig_val_0;
	orig_val_1.val = 1;
	orig_val_0.val = 0;
	
	if(semctl(MUTEX_ID, 0, SETVAL, orig_val_1) == -1)
	{
		printf("Error when initializing semp: MUTEX!\n");
		return -1;
	}
	
	if(semctl(P_TOKEN_ID, 0, SETVAL, orig_val_1) == -1)
	{
		printf("Error when initializing semp: P_TOKEN!\n");
		return -1;
	}
	
	if(semctl(C_TOKEN_ID, 0, SETVAL, orig_val_0) == -1)
	{
		printf("Error when initializing semp: C_TOKEN!\n");
		return -1;
	}
	
	return 0;
}

int getAndIncrement()
{
	int fd = open(TESTFILE_NAME, O_RDONLY);
	if(fd == -1) { printf("Error when opening file!\n"); return -1; }
	
	
	const int MAX_BUF_SIZE = 10;
	char buf[MAX_BUF_SIZE];
	size_t read_b = 0;
	size_t total = 1;
	char *str = malloc(total); str[0] = '\0';
	while((read_b = read(fd, buf, MAX_BUF_SIZE)) != 0)
	{
		// 1. Save old string.
		char *tmp = malloc(total + 1);
		strcpy(tmp, str);
		
		// 2. Allocate new buf with new size. 
		total += read_b;
		str = malloc(total + 1);
		
		// 3. Concat old str with new str.
		strcpy(str, tmp);
		strcat(str, buf);
		
		// 4. Release old buf.
		free(tmp);
	}
	
	counter = atoi(str);
	/**
	 * P.S. 该行输出的作用：用于测试同步成功后的操作原子性。
	 * 		换句话说，如果父进程与子进程成功互斥访问资源的话，应该同时具有两行输出，
	 *		下面这行输出的pid与 进入该函数之前输出的那行字符串 中的pid应该保持一致，
	 *		且两行输出中间没有其他的任何输出。
	 */
	printf("\tpid = %d accessed file, fd = %d, data read = %d", getpid(), fd, counter);
	printf(", write = %d \n", ++counter);
	close(fd);
	
	// Write to file.
	fd = open(TESTFILE_NAME, O_RDWR | O_CREAT | O_TRUNC);
	if(fd == -1) { printf("Error when opening file!\n"); return -1; }
	
	int size = counter / 10 + 1; 	// Calculate bytes' size.
	char w_str[size + 1];			// + '\0'
	sprintf(w_str, "%d", counter);
	write(fd, w_str, strlen(w_str));
	
	close(fd);
}
