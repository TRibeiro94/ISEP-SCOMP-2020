/*
 ============================================================================
 Name        : ex04.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	int fd[2];
	pipe(fd);
	FILE *f;

	pid_t p = fork();

	if (p == 0){
		close(fd[1]);
		char msg2[80];
		while (read(fd[0], msg2, 5)){
			printf("%s\n", msg2);
		}
		exit(0);
	} else {
		close(fd[0]);
		int b;
		f = fopen("/home/isep/scomp_1170691_1180512_1180809_1181444/PL2/ex04/src/file.txt", "r");
		char msgParent[20];
		while ((b = fread(msgParent, sizeof(char), 5, f)) > 0){
			write(fd[1], msgParent, b);
		}
		wait(NULL);
	}
	return 0;
}
