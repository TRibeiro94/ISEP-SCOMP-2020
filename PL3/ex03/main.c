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
#include <sys/time.h>
#define SIZE 1000000

void fill_array(int *array, int size){
    int i;
    srand(time(NULL) * getpid());
    for (i = 0; i < size; i++){
		array[i] = (rand() % 1000);
	}
}

void print_array(int *array){
	int i;
	size_t length = sizeof(array)/sizeof(array[0]);
	for(i = 0; i < length; i++){
		printf("Array[%d] = %d\n", i, array[i]);
	}
}

long getMicrotime(){	//returns the current time in microseconds
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int main(){
	int fd, *array, table[2], data_size = SIZE * sizeof(int);
	long start, finish; 
	pipe(table);
	
	shm_unlink ("/shm3");
	
	start = getMicrotime();
	fd = shm_open("/shm3", O_CREAT|O_EXCL|O_RDWR, S_IWUSR|S_IRUSR);
	if(fd == -1){
		printf("Error on attempting to open shared memory!\n");
		exit(EXIT_FAILURE);
	}
	
	int aux = ftruncate(fd, data_size);
	if(aux == -1){
		printf("Error while defining the size of shared memory!\n");
		exit(EXIT_FAILURE);
	}
	
	array = (int*) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(array == MAP_FAILED){
		printf("Error while mapping the shared memory object!\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid = fork();
	
	if(pid > 0){
		close(table[1]);
		read(table[0], &finish, sizeof(int));
		close(table[0]);
		
		double total = (double) (finish - start)/CLOCKS_PER_SEC;
		printf("\nTotal time: %f microseconds.\n", total);
		
	} else {
		close(table[0]);
		fill_array(array, SIZE);
		
		finish = getMicrotime();
		write(table[1], &finish, sizeof(int));
		close(table[1]);
		
		exit(0);
	}
	
	return 0;
}
