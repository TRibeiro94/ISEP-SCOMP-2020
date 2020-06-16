#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
int main(void){
	
	pid_t pid;
    sem_t *semaforo;
    int i;
	
    if ((semaforo = sem_open("semaforo", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
	
	pid = fork();
	if (pid == -1) {
            perror("Erro no fork");
            return 1;
    }
    
    for(i = 0; i < 8; i++){
		if (pid == 0) {
			printf("I’m the child\n");
			
			if (sem_post(semaforo) == -1){
				perror("Erro no incremento do semáforo");
				exit(1);
			}
			
			if(i == 7){
				exit(0);
			}
		}
		
		//parent
		else{
			if(i < 7){
				if (sem_wait(semaforo) == -1){
					perror("Erro no decremento do semáforo");
					exit(1);
				}
				
				printf("I’m the father\n");
			}
		}
	}

    if (sem_unlink("semaforo") == -1) {
        perror("Erro no processo");
        return 1;
    }
	
	return 0;
}
