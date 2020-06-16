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
    sem_t *index_access;
    sem_t *canRead;
    sem_t *canWrite;
    sem_t *x;
    int write_index, atendido = 0;
    int fd, r, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex12aaa", O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if((canRead = sem_open("sem2assd", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    if((canWrite = sem_open("sem1assd", O_CREAT)) == SEM_FAILED){
        printf("erro2");
    }
    if((index_access = sem_open("sem3assd", O_CREAT)) == SEM_FAILED){
        printf("erro3");
    }
    if((x = sem_open("sem4assd", O_CREAT)) == SEM_FAILED){
        printf("erro4");
    }
    sem_wait(index_access);
    write_index = shared_data->index;
    shared_data->index++;
    sem_post(index_access);
    while (!atendido){
        sem_wait(x);
        if(write_index == shared_data->atual_na_fila){
            atendido = 1;
        } else {
            sem_post(x);
        }
    }
    sem_post(canWrite);
    sem_wait(canRead);
    shared_data->atual_na_fila++;
    printf("O seu bilhete é %d\n", shared_data->prox_bilhete);
    sem_post(x);

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
}