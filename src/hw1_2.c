#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	execlp("./hello", "hello", NULL);
	printf("This is hw1_2.\n");
	
	return 0;
}

/*
Q: Why hw1_2 prints "Hello, World!", not "This is hw1_2."? 
A: Because execlp() has been used in this program. It replaces current process with new process.
*/
