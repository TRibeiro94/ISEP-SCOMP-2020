#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

int main(void){
	
	int child_processes = 5;
    int num_pipes = 6;
    int i;
    time_t t;

    int fd[num_pipes][2];
    
    for(i = 0; i < num_pipes; i++){
        if(pipe(fd[i]) == -1){
			perror("Pipe failed");
            return 1;
        }
    }
    
    int num_received, num_generated;
    for(i = 0; i < child_processes; i++){
		pid_t pid = fork();
	
		if(pid == -1){
			perror("fork failed");
			return 1;
		}
		
		if(pid == 0){
			close(fd[i][1]);
			
			srand((unsigned) time(&t) ^ getpid());
			
			num_generated = rand() % 500 + 1;
			
            read(fd[i][0], &num_received, sizeof(int));
            
            printf("Process: %d, the generated number is %d\n", getpid(), num_generated);
            
            close(fd[i][0]);
			close(fd[i+1][0]);
			
            if(num_generated > num_received){
				write(fd[i+1][1], &num_generated, sizeof(int));  
            }
            else{
				write(fd[i+1][1], &num_received, sizeof(int));
            }
            
            close(fd[i+1][1]);
			exit(0);
		}
	}
	
	// parent process
	close(fd[0][0]);
	
	srand((unsigned) time(&t) ^ getpid());
			
	num_generated = rand() % 500 + 1;
	
    printf("Process: %d, the generated number is %d\n", getpid(), num_generated);
    
	write(fd[0][1], &num_generated, sizeof(int));
	close(fd[0][1]);
	
	close(fd[5][1]);
	read(fd[5][0], &num_generated, sizeof(int));
	close(fd[5][0]);
	printf("The gratest random number is %d\n", num_generated);
	
	return 0;
}
