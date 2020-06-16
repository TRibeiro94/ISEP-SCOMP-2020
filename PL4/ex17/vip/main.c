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
    int index;
    int atual_na_fila;
    int quant_vips;
    int quant_normais;
    int quant_specials;
    int tipo_cliente;
    int nao_da;
    int lugares_livres;
} shared_data_type;

int main(void) {
    sem_t *index_access;
    sem_t *canRead;
    sem_t *canWrite;
    sem_t *x;
    sem_t *vip;
    int write_index, atendido = 0;
    int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex17", O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if((canRead = sem_open("ex17b", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    if((canWrite = sem_open("2ex17b", O_CREAT)) == SEM_FAILED){
        printf("erro2");
    }
    if((index_access = sem_open("3ex17b", O_CREAT)) == SEM_FAILED){
        printf("erro3");
    }
    if((x = sem_open("4ex17b", O_CREAT)) == SEM_FAILED){
        printf("erro4");
    }
    if((vip = sem_open("7ex17b", O_CREAT)) == SEM_FAILED){
        printf("erro5");
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
    shared_data->tipo_cliente = 1;
    sem_post(canWrite);//posso entrar?
    sem_wait(canRead);
    if (shared_data->nao_da == 1){
        printf("Esta cheio :c");
        shared_data->nao_da = 0;
        sem_post(x);
        return 0;
    }
    shared_data->atual_na_fila++;
    sem_post(x);
    printf("Estou a ver\n");
    sem_wait(vip);
    printf("Vou ter de sair!\n");

    //sem_wait(normal); para normal
    //sem_wait(special); para special
    //sem_wait(vip); para vips
}