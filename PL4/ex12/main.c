#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>

typedef struct {
	int nr_bilhetes;
    int prox_bilhete;
    int index;
    int atual_na_fila;
} shared_data_type;

int main(void) {
    sem_t *canRead;
    sem_t *canWrite;
    sem_t *index_access;
    sem_t *x;
    sem_unlink("sem2assd");
    sem_unlink("sem1assd");
    sem_unlink("sem3assd");
    sem_unlink("sem4assd");
    shm_unlink("/pl4ex12aaa");
    int fd, r, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex12aaa", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if ((canRead = sem_open("sem2assd", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    
    if ((canWrite = sem_open("sem1assd", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((index_access = sem_open("sem3assd", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if((x = sem_open("sem4assd", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    shared_data->nr_bilhetes = 5;
    shared_data->atual_na_fila = 1;
    shared_data->index = 1;
    shared_data->prox_bilhete = 0;
    
    while(shared_data->nr_bilhetes > 0){
        sem_wait(canWrite);
        shared_data->prox_bilhete = shared_data->atual_na_fila*2;
        shared_data->nr_bilhetes--;
        printf("Dei!\n");
        sem_post(canRead);

    }
    printf("Acabou\n");

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
}