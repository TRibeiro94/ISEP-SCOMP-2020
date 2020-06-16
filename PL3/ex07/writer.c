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
#define SIZE 10

void fill_array(int *array, int size){
    int i;
    srand(time(NULL) * getpid());
    for (i = 0; i < size; i++){
		array[i] = (rand() % 20 + 1);
	}
}

int main(){
	
	int fd, data_size = SIZE * sizeof(int), *array;
	fd = shm_open("/shm7", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
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
	
	fill_array(array, SIZE);
	
	return 0;
}
