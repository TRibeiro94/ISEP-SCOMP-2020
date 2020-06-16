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
#include <time.h>

#define DOORS 3
#define SIZE 200
#define START 0

typedef struct{
	int emptySeats;
	int filledSeats;
	int isPersonsInside;
}shared_data;


int createPersons(int amount);			//creates a random amount of persons for either leaving or entering
void personsEntering(shared_data *s);	//persons entering the train, prints out and updates the shared memory
void personsLeaving(shared_data *s);	//persons leaving the train, prints out and updates the shared memory
void semPost(sem_t *semaphore);			//sem_post and sem_wait functions with the error verification
void semWait(sem_t *semaphore);			//to avoid having the xact same 'if' condition multiple times	

int main(){
	pid_t pid[DOORS];
	shared_data *train;
	int i, fd, data_size = sizeof(shared_data);
	
	fd = shm_open("/shm_11", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	ftruncate(fd, data_size);
	
	train = (shared_data*) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	sem_t *s1;
	if ((s1 = sem_open("door1", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
	sem_t *s2;
	if ((s2 = sem_open("door2", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
	sem_t *s3;
	if ((s3 = sem_open("door3", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }	
	train->isPersonsInside = 0;
	
	for(i = START; i < DOORS; i++){
		pid[i] = fork();
		
		if(pid[i] == 0){
			semWait(s1);
			printf("Leaving from door [1].\n");
			personsLeaving(train);
			semPost(s1);
			
			semWait(s2);
			printf("Leaving from door [2].\n");
			personsLeaving(train);
			semPost(s2);
			
			semWait(s3);
			printf("Leaving from door [3].\n");
			personsLeaving(train);
			semPost(s3);
			
			exit(0);
		}else{
			semWait(s1);
			printf("Entering at door [1].\n");
			personsEntering(train);
			semPost(s1);
			
			semWait(s2);
			printf("Entering at door [2].\n");
			personsEntering(train);
			semPost(s2);
			
			semWait(s3);
			printf("Entering at door [3].\n");
			personsEntering(train);
			semPost(s3);
		}
	}
	
	sem_unlink("/door1");
	sem_unlink("/door2");
	sem_unlink("/door3");
	
	munmap(train, data_size);
	close(fd);
	shm_unlink("/shm_11");

	return 0;
}

int createPersons(int num){
	int persons;
	time_t t;
	srand((unsigned) time(&t));
	persons = rand() % num;
	return persons;
}

void personsEntering(shared_data *s){
	int numPersons;
	if(s->isPersonsInside == 0){
		numPersons = createPersons(SIZE);
		s->emptySeats -= numPersons;
		s->filledSeats += numPersons;
		s->isPersonsInside = 1;
	}else{
		numPersons = createPersons(s->emptySeats);
		s->emptySeats -= numPersons;
		s->filledSeats += numPersons;
	}
	printf("%d persons.\n", numPersons);
	sleep(1);
}

void personsLeaving(shared_data *s){
	int numPersons;
	if(s->isPersonsInside == 1){
		numPersons = createPersons(s->filledSeats);
		s->emptySeats += numPersons;
		s->filledSeats -= numPersons;
		if(s->emptySeats == 0){
			s->isPersonsInside = 0;
		}
	}else{
		return;
	}
	printf("%d persons.\n", numPersons);
	sleep(1);
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
