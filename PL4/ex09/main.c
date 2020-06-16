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
    sem_t *semaforo9a, *semaforo9b;
	
    if ((semaforo9a = sem_open("semaforo9a", O_CREAT| O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    if ((semaforo9b = sem_open("semaforo9b", O_CREAT| O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    	
	pid = fork();
	if (pid == -1) {
            perror("Erro no fork");
            return 1;
    } 
    
    if (pid == 0) {
		printf("Get beer\n");

		sem_post(semaforo9a);
// incrementa/abre o semáforo, caso o oturo processo o tenha fechado
		sem_wait(semaforo9b);
// decrementa/fecha o semáforo, esperando que o outro processo o abra
		
		printf("Eat and Drink2\n");
		exit(1);
	} else {
		printf("Get chips\n");

		sem_post(semaforo9b);
		sem_wait(semaforo9a);
		
		printf("Eat and Drink1\n");
	}

	sem_unlink ("/semaforo9a");
	sem_unlink ("/semaforo9b");
	
    return 0;
}
