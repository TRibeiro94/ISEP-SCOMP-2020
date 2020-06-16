#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#define SIZE 1000000

typedef struct{
	int valueA;
	int valueB;
} shared_data;

int main(){
	int table[2];
	pipe(table);
	int i, fd, data_size = sizeof(shared_data);
	shared_data *sd;
	
	shm_unlink ("/shm11"); //Removes the memory area from the file system.
	
	fd = shm_open("/shm11", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1){
		printf("Error on attempting to open shared memory!\n");
		exit(EXIT_FAILURE);
	}
	
	int aux = ftruncate(fd, data_size);
	if(aux == -1){
		printf("Error while defining the size of shared memory!\n");
		exit(EXIT_FAILURE);
	}
	
	sd = (shared_data*) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(sd == MAP_FAILED){
		printf("Error while mapping the shared memory object!\n");
		exit(EXIT_FAILURE);
	}
	
	sd->valueA = 8000;
	sd->valueB = 200;
	
	pid_t pid1 = fork();
	if(pid1 == -1){
		printf("Error on fork system call.");
		exit(EXIT_FAILURE);
	}
    
    if(pid1 > 0){
		for(i = 0; i < SIZE; i++){
			sd->valueA++;
			sd->valueB--;
		}
	}else{
		for(i = 0; i < SIZE; i++){
			sd->valueA--;
			sd->valueB++;
		}
		exit(0);
    }
    wait(NULL);		//guarantees the child process doesn't become an orphan
    
    printf("\nValue A: %d\nValue B: %d\n", sd->valueA, sd->valueB);
    
    pid_t pid2 = fork();	//now the child will check which one is the maximum and write it onto the parent
    int max;
    
    if(pid2 > 0){
		close(table[1]);
		read(table[0], &max, sizeof(int));
		close(table[0]);
	}else{
		close(table[0]);
		
		if(sd->valueA >= sd->valueB){
			max = sd->valueA;
		}else{
			max = sd->valueB;
		}
		
		write(table[1], &max, sizeof(int));
		close(table[1]);
		exit(0);
	}
	
	int unm = munmap(sd, data_size);
    if (unm == -1) {
        printf("Error disconnects the shared memory area!\n");
        exit(EXIT_FAILURE);
    }

    int closeFd = close(fd);
    if (closeFd == -1){
		printf("Error closing the file descriptor!\n");
        exit(EXIT_FAILURE);
	}
    
    printf("\nMax value (read by parent process through a pipe) = %d\n", max);
    return 0;
}
