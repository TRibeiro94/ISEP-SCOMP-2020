#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <wait.h>
#include <time.h>
#include <string.h>
#define CHILDS 8
#define RANDOM_NUM 100
#define EACH_CHILD_LIMIT 200
#define NUMBERS 1600

int main(){
	int i, j;
	sem_t *semaphore = sem_open("/sem_ex1",O_CREAT|O_EXCL, 0644, 1);
	if(semaphore == SEM_FAILED){
		printf("Error creating the semaphore!\n");
		exit(EXIT_FAILURE);
	} 
	
	//Fill the numbers.txt file with 1600 numbers, 200 for each child to read
	
	srand((unsigned) time(NULL));
	FILE * inputFile;
	inputFile = fopen("numbers.txt","w");
	for(i = 0; i < NUMBERS; i++){
		fprintf(inputFile, "%d ", rand()%RANDOM_NUM);
	}
	fclose(inputFile);
	
	FILE * reading;
	FILE * writing;
	
	for(i = 0; i < CHILDS; i++){
		pid_t pid = fork();
		if(pid == -1){
			printf("Error on fork system call.\n");
			exit(EXIT_FAILURE);
		}
		if(pid == 0){
			sem_wait(semaphore);
			reading = fopen("numbers.txt", "r");
			writing = fopen("output.txt", "a"); 
			
			int aux = i * EACH_CHILD_LIMIT;
			
			for(j = aux; j < (EACH_CHILD_LIMIT + aux); j++){
				int num = 0;
				fscanf(reading,"%d", &num);
				if(j >= aux && j <= aux + EACH_CHILD_LIMIT)
					fprintf(writing, "%d\n", num);
			}
			
			fclose(reading);
			fclose(writing);
			sem_post(semaphore);
			exit(0);
		}
	}

	for(i = 0; i < CHILDS; i++){
		wait(NULL);
	}
	
	int num;
	FILE * output2;
	output2 = fopen("output.txt","r");
	if(output2 == NULL){
		printf("Error opening file.");
		exit(EXIT_FAILURE);
	}
	while(fscanf(output2, "%d\n", &num) != EOF){
		printf("%d\n", num);
	}
	
	fclose(output2);
	sem_unlink("/sem_ex1");
	
	return 0;
}
