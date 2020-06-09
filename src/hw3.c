#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>

void *ThreadFn_fill(void *param);

typedef struct {
	int threadIdx;
	int noThread;
	int *data;
	int *filledBy;
	int size;
}ThreadInfo;

void *ThreadFn_fill(void *param)
{
	ThreadInfo *info = (ThreadInfo*) param;

	int i = 0;
	for(i = info->threadIdx; i < info->size; i += info->noThread){
		info->data[i] = i;
		info->filledBy[i] = info->threadIdx;
	}
	pthread_exit(0);

	return NULL;
}

int main(int argc, char *argv[])
{	
	pthread_attr_t attr;
	int size = atoi(argv[1]);
	int noThread = atoi(argv[2]);
	
	pthread_t *tid = (pthread_t*)malloc(noThread*sizeof(pthread_t));
	ThreadInfo *threadInfo = (ThreadInfo*)malloc(noThread*sizeof(ThreadInfo));
	int *data = (int*)malloc(size*sizeof(int));
	int *filledBy = (int*)malloc(size*sizeof(int));

	if (tid == NULL || threadInfo == NULL || data == NULL || filledBy == NULL)
	{
		printf("Failed to allocate memory\n");
		return 0;
	}
	
	pthread_attr_init(&attr);

	int t;
	for (t = 0; t < noThread; t++) {
		threadInfo->threadIdx = t;
		threadInfo->size = size;
		threadInfo->noThread = noThread;
		threadInfo->data = data;
		threadInfo->filledBy = filledBy;
		pthread_create(&tid[t], &attr, ThreadFn_fill, &threadInfo[t]);
	}
	for (t = 0; t < noThread; t++) {
		pthread_join(tid[t], NULL);
	}
	for (t = 0; t < size; t++) {
		printf("data[%d] = %d (filled by thread %d)\n", t, threadInfo->data[t], threadInfo->filledBy[t]);
	}
	free(threadInfo->filledBy);
	threadInfo->filledBy = NULL;
	free(threadInfo->data);
	threadInfo->data = NULL;
	free(threadInfo);
	threadInfo = NULL;
	free(tid);
	tid = NULL;

	return 0;
}
