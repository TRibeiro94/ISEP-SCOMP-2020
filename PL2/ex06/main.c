#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

int main(void){
	
	int i, j, status, vec_size = 5;
	
	int vec1[vec_size];
	int vec2[vec_size];
	time_t t;
	
	srand((unsigned) time(&t));

	for(i = 0; i < vec_size; i++) {
		vec1[i] = rand() % 50;
		vec2[i] = rand() % 50;
    }
    
    int fd[2];
	
	if(pipe(fd) == -1){ 
		perror("Pipe failed");
        return 1; 
    }
    
    for(i = 0; i < 5; i++){
		pid_t pid = fork();
		if(pid == 0){
			close(fd[0]);
			int inicio = 1 * i;
            int fim = 1 * (i + 1);
            int tmp = 0;
            
            for(j = inicio; j < fim; j++){
				tmp += vec1[j] + vec2[j];
			}
			
			write(fd[1], &tmp, sizeof(tmp));
			close(fd[1]);
			exit(0);
		}
	}
	
	int final_sum = 0;
	
	close(fd[1]);
	
	for(i = 0; i < 5; i++){
        wait(&status);
        
        int partial_sum = 0;
        
		read(fd[0], &partial_sum, sizeof(partial_sum));
		
		printf("%d\n", partial_sum);
		
		final_sum += partial_sum;
    }
    
    close(fd[0]);
    
    for(i = 0; i < vec_size; i++){
		printf("%d ", vec1[i]);
	}
	
	printf("\n");
	
	for(i = 0; i < vec_size; i++){
		printf("%d ", vec2[i]);
	}
	
	printf("\n");
    
    printf("Total: %d\n", final_sum);
	
	return 0;
}
