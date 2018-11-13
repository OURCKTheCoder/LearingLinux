#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include<unistd.h>

// Buffer
static int buf[1];

// Semaphore
sem_t mutex;
sem_t token_pdser;
sem_t token_csmer;

// Threads
pthread_t consumer, producer;

// Functions
void *pdser_routine(void *args);
void *csmer_routine(void *args);
int Initialize();

// ---------------------------------------------------------

int main()
{
	Initialize();
	
	return 0;
}

int Initialize()
{
	// 1. Initialize semp.
	if(sem_init(&mutex, 0, 1) != 0) printf(" [!] Initialize failed: mutex\n");
	if(sem_init(&token_pdser, 0, 1) != 0)  printf(" [!] Initialize failed: tpken_pdser\n");
	if(sem_init(&token_csmer, 0, 0) != 0)  printf(" [!] Initialize failed: tpken_csmer\n");

	// 2. Initialize threads.
	pthread_create(&producer, NULL, pdser_routine, NULL);
	pthread_create(&consumer, NULL, csmer_routine, NULL);
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
	
	return 0;
}

void *pdser_routine(void *args)
{
	while(1)
	{
		sem_wait(&token_pdser);
		sem_wait(&mutex);
		
		time_t t;
		srand((unsigned) time(&t));
		int val = rand();
		buf[0] = val;
		printf("o\t - Producer puts num = %d\n", val);
		
		sem_post(&mutex);
		sem_post(&token_csmer);
		sleep(1);
	}
}

void *csmer_routine(void *args)
{
	while(1)
	{
		sem_wait(&token_csmer);
		sem_wait(&mutex);
		
		int val = buf[0];
		printf("  o\t - Consumer gets num = %d", val);
		printf(", +1 = %d\n", ++val);

		sem_post(&mutex);
		sem_post(&token_pdser);
		//sleep(1);
	}
}
