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

typedef struct{
	char strings[50][80];
}shared_data;

int main(){
	
	shared_data *msg;
	int fd, data_size = sizeof(shared_data);
	
	fd = shm_open("/shm_3", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1){
		printf("Error on attempting to open shared memory!\n");
		exit(EXIT_FAILURE);
	}
	
	int aux = ftruncate(fd, data_size);
	if(aux == -1){
		printf("Error while defining the size of shared memory!\n");
		exit(EXIT_FAILURE);
	}
	
	msg = (shared_data*) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	sem_t *s = sem_open("/sem_3",O_CREAT|O_EXCL, 0644, 1);
	if(s == SEM_FAILED){
		printf("Error creating the semaphores.\n");
		exit(EXIT_FAILURE);
	}
	
	sprintf(msg->strings[0], "%s %d", "\nI'm the Father - with PID ", getpid());
	
	pid_t pid = fork();
	wait(NULL);
	
	if(pid == 0){
		sem_wait(s);
		printf("I'm the child %d. My father is %d.\n", getpid(), getppid());
		sleep(2);			
		sem_post(s);
		exit(0);
	}
	
	sem_wait(s);
	printf("%s\n",msg->strings[0]);
	sem_post(s);
	
	sem_unlink("/sem_3");
	shm_unlink("/shm_3");
	
	return 0;
}
