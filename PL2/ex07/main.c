#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#define ARRAY_SIZE 1000
 
int main(){
	
	time_t t;
	pid_t pid[5];
	
	int vec1[ARRAY_SIZE];
	int vec2[ARRAY_SIZE];
	int result[ARRAY_SIZE];
	int table [5][2];	//5 descriptor tables, one for each child
	int status, i, j, aux, aux2;
	srand ((unsigned) time (&t));

	for (i = 0; i < ARRAY_SIZE; i++){
		vec1[i] = rand() % 100;			//fills the arrays of 1k size with values from [0-100]
		vec2[i] = rand() % 100;
	}

	for (i = 0; i < 5; i++){
		pipe(table[i]);
		pid[i] = fork();

		if (pid[i] < 0){
			printf("Error on fork system call.");
			exit(1);
			
		} else if(pid[i] == 0){
			close(table[i][0]);
			
			int start = 200 * i;		
			int end = (i+1) * 200;
			
			for (j = start; j < end; j++){
				aux = vec1[j] + vec2[j];
				write(table[i][1], &aux, sizeof(int));
			}
			close(table[i][1]);
			exit(1);
			
		} else { 
			wait(&status);	
			close(table[i][1]);
			
			int start = 200 * i;		
			int end = (i+1) * 200;
			
			for(j = start; j < end; j++){
				read(table[i][0], &aux2, sizeof(int));
				result[j] = aux2;
				printf("result[%d] = %d\n", j, result[j]);	
			}
			close(table[i][0]);
		}
	}
	return 0;
}

