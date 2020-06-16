#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <wait.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#define CHILDS 8
#define EACH_CHILD_LIMIT 200
#define NAME_SIZE 50
#define NUMBERS 1600
int main(void){
	
    pid_t pid[8];
    int status[8];
    int *offset = NULL;
    int fd, data_size = sizeof(int);
    sem_t *semaforo[8];
	char nome_semaforo[8][NAME_SIZE];
	int i, j;
	
	shm_unlink("/offset");
	
	fd = shm_open("/offset", O_CREAT|O_EXCL|O_RDWR, S_IWUSR|S_IRUSR);
	if (fd == -1) exit(1);
	
	int aux = ftruncate(fd, data_size);
	if (aux == -1) exit(1);
	
	offset = (int *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (offset == MAP_FAILED) exit(1);
	
	FILE * inputFile;
	inputFile = fopen("numbers.txt","w");
	for(i = 0; i < NUMBERS; i++){
		fprintf(inputFile, "%d ", i+1);
	}
	fclose(inputFile);
	
	for(j = 0; j < 8; j++){
		sprintf(nome_semaforo[j], "sem%d", j);
		if ((semaforo[j] = sem_open(nome_semaforo[j], O_CREAT, 0644, 0)) == SEM_FAILED){
			perror("Error creating the semaphore!\n");
			exit(1);
		}
	}

	remove("output.txt");
	
	*offset = 0;
	
    for (i = 0; i < 8; i++){
		
        pid[i] = fork();
        
        if (pid[i] == -1){
            perror("Error fork");
            return 1;
        }
        
        if (pid[i] == 0){
            if (sem_wait(semaforo[i]) == -1){
                perror("Error sem wait");
                exit(1);
            }
			
			FILE *inputfile;
            FILE *outputfile;
			
            inputfile = fopen("numbers.txt", "r");
            outputfile = fopen("output.txt", "a");

            if (outputfile == NULL || inputfile == NULL){
                perror("Error opening file.");
                exit(1);
            }
            
            int aux = i * EACH_CHILD_LIMIT;
            
            fseek(inputfile, *offset, SEEK_CUR);
			
			for(j = aux; j < (EACH_CHILD_LIMIT + aux); j++){
				int num = 0;
				fscanf(inputfile,"%d", &num);
				if(j >= aux && j <= aux + EACH_CHILD_LIMIT)
					fprintf(outputfile, "%d\n", num);
			}
			
			*offset = ftell(inputfile);

            int closeoutputfile = fclose(outputfile);
            int closeinputfile = fclose(inputfile);

            if (closeoutputfile == -1 || closeinputfile == -1){
                perror("Error closing file");
                return -1;
            }

            if (sem_post(semaforo[i])){
                perror("Error sem post");
                exit(1);
            }

            exit(0);
        }
    }
    
    
    // parent
    for (i = 0; i < 8; i++) {
		if (sem_post(semaforo[i])){
            perror("Error sem post");
            exit(1);
        }
		
        if (waitpid(pid[i], &status[i], 0) == -1){
            perror("Error waiting for process");
            return 1;
        }
    }
    
    int num;
	FILE * outputfile2;
	outputfile2 = fopen("output.txt","r");
	if(outputfile2 == NULL){
		printf("Error opening file.");
		exit(EXIT_FAILURE);
	}
	while(fscanf(outputfile2, "%d\n", &num) != EOF){
		printf("%d ", num);
	}
	printf("\n");
	
	fclose(outputfile2);
	
	for(j = 0; j < 8; j++){
		if (sem_unlink(nome_semaforo[j]) == -1){
			perror("Error waiting for process");
			return 1;
		}
	}
	
	return 0;
}
