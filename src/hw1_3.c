#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int child_pid = fork();
	
	if (child_pid == 0)
	{
		execlp("./hello", "hello", NULL);
	}
	else
	{
		wait();
		printf("This is hw1_3.");
	}

	return 0;
}

/*
Q: If remove wait() and run again, what's the difference? Describe why.
A: No difference in result. Originally parent process waited for the termination of child process.
But after removing wait(), parent process executed its process without waiting for child process to finish.
*/
