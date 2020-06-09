#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

char shared_mesg[64] = "Greetings!";

pthread_mutex_t wrt = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int reader_count = 0;

int bThreadContinue = TRUE;

typedef struct tThreadInfo {
	int thread_idx;
	int delay;
} ThreadInfo;

void* reader(void *param) {
	ThreadInfo *info = (ThreadInfo*)param;
	
	do {
		wait(mutex);
		reader_count++;
		if (reader_count == 1)
			wait(wrt);
		signal(mutex);

		printf("[reader %d] shared_mesg = %s\n", info->thread_idx, shared_mesg);

		wait(mutex);
		reader_count--;
		if (reader_count == 0)
			signal(wrt);
		signal(mutex);

		sleep(info->delay);
	} while (bThreadContinue == TRUE);

	pthread_exit(0);
	return NULL;	
}

void* writer(void *param) {
	ThreadInfo *info = (ThreadInfo*)param;
	const int no_mesg_samples = 10;
	static char mesg_samples[][64] = { "Hello.", "Good morning.", "Nice to see you.", "Glad to see you.", "Happy new year!", "Good luck.", "God bless you.", "Welcome to HGU.", "Have a nice day.", "Everything is gonna be OK." };
	
	srand(time(NULL));
	do {
		int noMesg = rand()%no_mesg_samples;
		wait(wrt);
		sprintf(shared_mesg, "%c", mesg_samples[noMesg][64]);
		printf("shared_mesg = %s (written by writer %d)\n", shared_mesg, info->thread_idx);
		signal(wrt);

		sleep(info->delay);
	} while (bThreadContinue == TRUE);

	pthread_exit(0);
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_attr_t attrR;
	pthread_attr_t attrW;
	srand(time(NULL));
	int duration = atoi(argv[1]);
	int numReaders = atoi(argv[2]);
	int numWriters = atoi(argv[3]);

	ThreadInfo *threadInfoW = (ThreadInfo*)malloc(numReaders*sizeof(ThreadInfo));
	ThreadInfo *threadInfoR = (ThreadInfo*)malloc(numWriters*sizeof(ThreadInfo));
	pthread_t *tidW = (pthread_t*)malloc(numReaders*sizeof(pthread_t));
	pthread_t *tidR = (pthread_t*)malloc(numWriters*sizeof(pthread_t));

	if (threadInfoW == NULL || threadInfoR == NULL || tidW == NULL || tidR == NULL) {
		printf("Failed to allocate memory!");
	}

	int t = 0;
	for (t = 0; t < numReaders; t++) {
		threadInfoW->thread_idx = t;
		threadInfoW->delay =  rand()%3+3;
		pthread_create(&tidR[t], &attrR, writer, &threadInfoW[t]);
	}
	for (t = 0; t < numWriters; t++) { 
		threadInfoR->thread_idx = t;
		threadInfoR->delay = rand()%3+3;
		pthread_create(&tidW[t], &attrW, reader, &threadInfoR[t]);
	}

	sleep(duration);
	bThreadContinue = FALSE;

	for (t = 0; t < numReaders; t++) {
		pthread_join(tidR[t], NULL);
	}
	for (t = 0; t < numWriters; t++) {
		pthread_join(tidW[t], NULL);
	}

	free(tidR);
	free(tidW);
	free(threadInfoR);
	free(threadInfoW);
	tidR = NULL, tidW = NULL, threadInfoR = NULL, threadInfoW = NULL;

	printf("Bye!\n");

	return 0;
}
