#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include "struct.h"
#include "struct2.h"

int main(void){
	int fd = shm_open("/shm_ex10", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if(fd == -1){
    perror("Opening fd error \n");
		exit(EXIT_FAILURE);
	}

	int size = sizeof(SharedData);

	if(ftruncate(fd, size) == -1){
		perror("ftruncate ERROR\n");
		exit(0);
	}

	SharedData *sd = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(sd == NULL){
		printf("MMAP ERROR\n");
		exit(0);
	}
	
	sem_t *sem;
	
	sem = sem_open("/sem_ex10", O_CREAT, 0644, 1);
	if(sem == SEM_FAILED){
		printf("Opening fd error\n");
		return 0;
	}

	sem_wait(sem);

	int number;

	printf("Insert number:\n");

	scanf("%d", &number);
	fflush(0);

	if(sd->index == 0){
		printf("No records available.\n");
	}
	else{
		int i;
		for(i = 0; i < sd->index; i++){
			record *r = &(sd->entry[i]);
			if(r->number == number){
				printf("Record: %d\nNumber: %d  Name: %s  Address: %s\n", i+1, r->number, r->name, r->address);
				printf("\n");
			}
		}
	}

	sem_post(sem);
	
	sem_unlink("/sem_ex10");

	munmap(sd, size);
	close(fd);

	return 0;
}
