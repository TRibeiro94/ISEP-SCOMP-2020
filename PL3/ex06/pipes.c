#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#define size 10000
int main(void){
	int i, vec[size];
	clock_t start_pipes, end_pipes;
	
	int fd[2];
	
	if(pipe(fd) == -1){ 
		perror("Pipe failed");
        return 1; 
    }
	
	srand((unsigned) time(NULL));

	for(i = 0; i < size; i++){
		vec[i] = rand() % 10;
	}
		
	start_pipes = clock();
	
	pid_t pid;

	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		
		int x;
		
		close(fd[0]);
		
		for(x = 0; x < size; x++){
			
			write(fd[1], &vec[x], sizeof(vec[x]));
		}
		close(fd[1]);
		exit(0);
	}
	
	//parent
	
	wait(NULL);
	
	close(fd[1]);
	
	int array_to_get[size];
	
	for(i = 0; i < size; i++){
		
		read(fd[0], &array_to_get[i], sizeof(array_to_get[i]));
	}
	close(fd[0]);
	
	end_pipes = clock();
	
	float time_pipes = (float) (end_pipes - start_pipes) / CLOCKS_PER_SEC;

	printf("Time of pipes process: %f\n", time_pipes);

	return 0;
}
