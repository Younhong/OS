#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

# define BUFFER_SIZE 10

typedef struct {
	int data[BUFFER_SIZE];
	int in, out;
} Buffer;

void producer(Buffer *buffer)
{
	srand(time(NULL));
	int i;
	for (i = 0; i < 100; i++) {
		int x = rand()%100;
		printf("Producer inserting %d\n", x);
		
		while (((buffer->in + 1)%BUFFER_SIZE) == buffer->out);
		buffer->data[buffer->in] = x;
		buffer->in = (buffer->in+1)%BUFFER_SIZE;
		
		sleep(rand()%2+1);
	}
}

void consumer(Buffer *buffer)
{
	srand(time(NULL));
	int i;
	for (i = 0; i < 100; i++) {
		while (buffer->in == buffer->out);
		int y = buffer->data[buffer->out];
		buffer->out = (buffer->out + 1)%BUFFER_SIZE;
		printf("\t\tConsumer deleted %d\n", y);
		sleep(rand()%4+1);
	}
}

int main()
{	
	int size = sizeof(Buffer);
	int shm_id = shmget(IPC_PRIVATE, size, S_IRUSR|S_IWUSR);
	Buffer *buffer = shmat(shm_id, NULL, 0);
	buffer->in = buffer->out = 0;
	shmdt(buffer);

	int child_pid = fork();			
	if (child_pid == 0) {
		Buffer *buffer_child = shmat(shm_id, NULL, 0);
		consumer(buffer_child);
		shmdt(buffer_child);
	}
	else {
		Buffer *buffer_parent = shmat(shm_id, NULL, 0);
		producer(buffer_parent);
		shmdt(buffer_parent);
		shmctl(shm_id, IPC_RMID, NULL);
	}
	return 0;
}
