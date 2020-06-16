#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <string.h>

int main(void) {
    pid_t pid;
    sem_t *semaforo5;
	
    if ((semaforo5 = sem_open("semaforo5", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
	
	pid = fork();
	if (pid == -1) {
            perror("Erro no fork");
            return 1;
    }
    if (pid == 0) {
		printf("I’m the child\n");
		if (sem_post(semaforo5) == -1){
            perror("Erro no incremento do semáforo");
            exit(1);
        }
		exit(0);
        
	} else {
		if (sem_wait(semaforo5) == -1){
            perror("Erro no decremento do semáforo");
            exit(1);
        }
		printf("I’m the father\n");	 
	}
   

    if (sem_unlink("semaforo5") == -1) {
        perror("Erro no processo");
        return 1;
    }
    
	sem_unlink("/semaforo5");
	
    return 0;
}
