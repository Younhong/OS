#include <stdio.h>

int main()
{
	fork();
	fork();
	fork();
	printf("Hello, World!\n");
	
	return 0;
}

/*
Q1: Relation between the number of fork() and the number of messages. And describe why.
A1: number of messages = 2 to the power of the number of fork(). In this example, 3 fork() was used and 8 messages were printed. fork() copies processes. 
First fork() copies messages once, second fork() copies messages including copied one. and next fork() will copy all the message which means double amount of messages.
Q2: How many times fork() was called by processes?
A2: fork() was called 7 times.
Q3: If replace fork() with printf("fork() = %d\n", fork());, then how many times does it print zero? Why?
A3: If fork() was replaced by printf statements above, then it does not copy messages, but prints the integer value of fork(), which is zero. fork() was used 3 times in this program, so each replacement will print 0. So the program will print zero  3 times.
*/
