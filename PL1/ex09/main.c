#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(){
	int i;
	int min, max;
	pid_t arrayPids[10];
	int size = sizeof(arrayPids) / sizeof(arrayPids[0]);

	for(i = 0; i < size; i++){
		
		arrayPids[i] = fork();
		
		if(arrayPids[i] == -1){
			printf("Error forking.\n");
			exit(1);
		}
		
		if(arrayPids[i] == 0){
			min = (100*i) + 1;
			max = 100 + (100*i);
			printf("Child process with pid = %d\n------------", getpid());
			
			for(i = min; i <= max; i++){
				printf("\n%d", i);
				
			}
			printf("\n------------\n");
			exit(0);
		}
				
	}
	
	for(i = 0; i < size; i++){
		waitpid(arrayPids[i], NULL, 0);			//waits childs to finish
	}
	printf("Parent process ended.\n");
	return 0;
}
