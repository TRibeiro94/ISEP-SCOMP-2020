#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include "struct.h"
#define size 1000000
int main(void){
	int data_size = sizeof(Data_type), i, vec[size];
	clock_t start_shm, end_shm;
	
	shm_unlink("/shm_ex06");

	int fd = shm_open("/shm_ex06", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Opening shared memory ERROR.\n");
		exit(EXIT_FAILURE);
	}

	if(ftruncate(fd, data_size) == -1){
		perror("Space in Shared Memory ERROR\n");
		exit(EXIT_FAILURE);
	}

	Data_type* shared_data = (Data_type*) mmap(NULL, data_size, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);
	if(shared_data == MAP_FAILED){
		perror("Error maping the object.\n");
		exit(EXIT_FAILURE);
	}
	
	srand((unsigned) time(NULL));

	for(i = 0; i < size; i++){
		vec[i] = rand() % 10;
	}
	
	start_shm = clock();

	pid_t pid;

	if((pid = fork()) == -1){
		perror("Failed fork\n");
		exit(EXIT_FAILURE);
	}

	if(pid == 0){
		
		int x;
		
		for(x = 0; x < size; x++){

			shared_data->num[x] = vec[x];
		}

		exit(0);
	}
	
	//parent
	
	wait(NULL);
	
	end_shm = clock();

	float time_shm = (float) (end_shm - start_shm) / CLOCKS_PER_SEC;

	printf("Time of shared memory process: %f\n", time_shm);

	if(munmap(shared_data, data_size) == -1){
		perror("Cant read object.\n");
		exit(EXIT_FAILURE);
	}

	if(close(fd) == -1){
		perror("Cant close object.\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
