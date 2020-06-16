#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#define PROCESSES 3
#define START 0

/*sem_post and sem_wait functions with the error verification
  to avoid having the xact same 'if' condition multiple times
*/
void semPost(sem_t *semaphore);
void semWait(sem_t *semaphore);

int main(){
	int i, status;

	sem_t *s1 = sem_open("/sem1",O_CREAT|O_EXCL, 0644, 0);
	sem_t *s2 = sem_open("/sem2",O_CREAT|O_EXCL, 0644, 0);
	sem_t *s3 = sem_open("/sem3",O_CREAT|O_EXCL, 0644, 0);
	
	if(s1 == SEM_FAILED || s2 == SEM_FAILED || s3 == SEM_FAILED){
		printf("Error creating the semaphores.\n");
		exit(EXIT_FAILURE);
	} 
	
	for(i = START; i < PROCESSES; i++){
		
		pid_t pid = fork();
		if(pid == -1){
			perror("Error on fork system call.\n");
            exit(EXIT_FAILURE);
		}
		
		if(pid == 0){
			if(i == 0){				//process 1 (prints "Sistemas" and "a")
				printf("\nSistemas ");
				fflush(stdout);	
				semPost(s2);
				
				semWait(s1);
				printf("a ");
				semPost(s2);
				
				exit(EXIT_SUCCESS);
			}
			if(i == 1){				//process 2 (prints "de" and "melhor")
				semWait(s2);
				printf("de ");
				fflush(stdout);	
				semPost(s3);
				
				semWait(s2);
				printf("melhor ");
				semPost(s3);
				
				exit(EXIT_SUCCESS);
			}
			if(i == 2){				//process 3 (prints "Computadores" and "disciplina")
				semWait(s3);
				printf("Computadores - ");
				fflush(stdout);
				semPost(s1);
				
				semWait(s3);
				printf("disciplina!\n");
				
				exit(EXIT_SUCCESS);
			}
		}
	}
	
	for(i = START; i < PROCESSES; i++){		//waits for all child processes before unlinking the 3 semaphores
		wait(&status);
	}
	
	sem_unlink("/sem1");
	sem_unlink("/sem2");
	sem_unlink("/sem3");
	
	return EXIT_SUCCESS;
}

void semPost(sem_t *semaphore){
	if(sem_post(semaphore) == -1){
		printf("sem_post error.\n");
		exit(EXIT_FAILURE);
	}
}

void semWait(sem_t *semaphore){
	if(sem_wait(semaphore) == -1){
		printf("sem_wait error.\n");
		exit(EXIT_FAILURE);
	}
}

/*
 * In multi-process or multi-thread code. The buffer doesn't always flush on a 
 * call to a printf(), so to know the true order of execution of multiple 
 * processes the fflush is used after every print.
 */
