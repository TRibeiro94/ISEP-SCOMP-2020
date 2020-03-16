#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#define ARRAY_SIZE 1000

int main(){
	
	time_t t;
	pid_t pid[5];
	srand ((unsigned) time (&t));
	
	int array[ARRAY_SIZE];	
	int result[ARRAY_SIZE];	
	int max = 0;	
	int status[5];
	int i, j;
								
	for (i = 0; i < ARRAY_SIZE; i++){		
		array[i] = rand() % 255;			//fills the array of 1k size with values from [0-255]
	
	}

	for(i = 0; i < 5; i++){					//will create 5 child processes
		pid[i] = fork();	
		
		if(pid[i] < 0){
			printf("Error on fork system call.");
			exit(1);
			
		}else if(pid[i] == 0){
			
			int start = 200 * i + 1;		//will start at 1, 201, 401, 601, 801
			int end = (i + 1) * 200; 		//will end at 200, 400, 600, 800, 1000
			if(array[0] > max){				//checks first position
				max = array[0];					
			}
			for(j = start; j < end; j++){
				if(array[j] > max){			//determines the max value in current range
					max = array[j];
				}
			}
			printf("Child PID [%d] max value = %d\n", getpid(), max);
			exit(max);
		}
	}
	
	for(i = 0; i < 5; i++){
		waitpid(pid[i], &status[i], 0);			//parent process waits the 5 childs
	}
	
	for(i = 0; i < 5; i++){
		if(WIFEXITED(status[i])){				//if child process exited successfully
			if(WEXITSTATUS(status[i]) > max){	//confirms max value out of the 5 discovered by the child processes
				max = WEXITSTATUS(status[i]);
			}
		}
	}
	printf("\nMaximum value in the array = %d\n\n", max);
	
	pid_t p = fork();							//line b requested fork
	
	if(p < 0){
		printf("Error on fork system call.");
		exit(1);
	}else if(p == 0){							//child process will iterate 1st half of the array
		for(i = 0; i < ARRAY_SIZE/2; i++){
			result[i]=(array[i]/max)*100;
			printf("result[%d] = %d\n", i, result[i]);
		}
	}else{
		wait(NULL);										//parent process will iterate 2nd half of the array after child process
		for(i = ARRAY_SIZE/2; i < ARRAY_SIZE; i++){
			result[i]=((int) array[i]/max)*100;
			printf("result[%d] = %d\n", i, result[i]);
		}
	}
	
	return 0;
}
