#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
	fork(); fork(); fork();
	printf("SCOMP!\n");
	return 0;
}

/*
 * a)
 * Q: How many processes are created by this code?
 * A: 8
 * 
 * b)
 * Q: Draw a process tree that describes the processes created.
 * A: (.png file in ex3 folder)
 * 
 * c)
 * Q: How many times is “SCOMP” printed?
 * A: 8
 * 
 */
