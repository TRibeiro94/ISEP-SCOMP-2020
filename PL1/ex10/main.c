#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#define ARRAY_SIZE 2000

int main(){
	
	time_t t;
	pid_t p;
	srand ((unsigned) time (&t));
	
	int array[ARRAY_SIZE];			
	int n;	//target number
	int status, i, j;
								
	for (i = 0; i < ARRAY_SIZE; i++){
		array[i] = rand() % 1000;
	}
	n = rand() % 1000;							//generates a random number
	printf("\nGenerated target number : %d\n", n);
	
	for(i = 0; i < 10; i++){				//will create the 10 child processes
		p = fork();
		
		if(p < 0){
			printf("Error on fork system call.");
			exit(1);
		}else if(p == 0){
			int startRange = 200 * i + 1;
			int endRange = (i + 1) * 200; 	//will end at 200, 400, 600, etc
			if(array[0] == n){
				exit(0);					//checks if it is in the first position
			}
			for(j = startRange; j < endRange; j++){
				if(array[j] == n){
					printf("Original index %d found by child with PID [%d].\n", j, getpid());
					exit(j - startRange);	//because can't exit with numbers > 255 so this way, it's always in a 200 value range
				}
			}						
			exit(255);			//if 'n' is not found, return 255
		}
	}
	
	printf("\nIndexes where '%d' was found:\n", n);
	
	for(i = 0; i < 10; i++){					//will wait for the 10 childs
		wait(&status);
		
		if(WIFEXITED(status) && WEXITSTATUS(status) != 255){	//if exit was successful
			printf("[0-200] range index = %d\n", WEXITSTATUS(status));				//print the index
		}
	}
	
	return 0;
}
