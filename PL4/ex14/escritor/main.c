#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>

typedef struct {
	int nrLeitores;
    int nrEscritores;
    char frase[50];
} shared_data_type;

int main(void) {
    sem_t *nrEscritores;
    sem_t *podeLer;
    sem_t *podeEscrever;
    sem_unlink("ex14sem1b");
    sem_unlink("ex14sem2b");
    sem_unlink("ex14sem3b");
    shm_unlink("/pl4ex14c");
    int fd, r, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex14c", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if ((nrEscritores = sem_open("ex14sem1b", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((podeLer = sem_open("ex14sem2b", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((podeEscrever = sem_open("ex14sem3b", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    int i;
    sem_wait(nrEscritores);
    shared_data->nrEscritores++;
    sem_post(nrEscritores);
    sem_wait(podeEscrever);
    time_t timeinfo;
	time(&timeinfo);
    char *time_str = ctime(&timeinfo);
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "My PID is %d - %s\n", getpid(), time_str);
    strcpy(shared_data->frase, buffer);
    sleep(20);
    sem_wait(nrEscritores);
    shared_data->nrEscritores--;
    if(shared_data->nrEscritores == 0){
        for (i = 0; i < shared_data->nrLeitores; i++){
            sem_post(podeLer);
        }
    } else {
        sem_post(podeEscrever);
    }
    sem_post(nrEscritores);

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
}