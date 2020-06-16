/*
 ============================================================================
 Name        : 2ex08.c
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
#include <string.h>

typedef struct{
	char msg[10];
	int round;
} strA;

int main(void) {
	int i;
	int fd[2];
	pipe(fd);
	pid_t arrayPids[10];

	for(i = 0; i < 10; i++){

		arrayPids[i] = fork();

		if (arrayPids[i] == -1){
			printf("Error!\n");
			exit(1);
		}

		if(arrayPids[i] == 0){
			close(fd[1]);
			strA toRead;
			read(fd[0], &toRead, sizeof(toRead));
			printf("%s - %d\n", toRead.msg, toRead.round);
			exit(toRead.round);
		}
	}
	close(fd[0]);

	strA st;
	char ab[10] = "Win";
	strcpy(st.msg, ab);
	for(i = 0; i < 10; i++){
		st.round = i + 1;
		write(fd[1], &st, sizeof(st));
		sleep(2);
	}

	int exit_value;
	for(i = 0; i < 10; i++){
		exit_value = 0;
		pid_t p = wait(&exit_value);
		printf("Process: %d, Round: %d\n", p, WEXITSTATUS(exit_value));
	}
	return 0;
}
