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
    sem_t *canRead;
    sem_t *canWrite;
    sem_t *index_access;
    sem_t *x;
    sem_t *normal;
    sem_t *vip;
    sem_t *special;
    //sem_unlink("sem3assd");
    int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex17", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if ((canRead = sem_open("ex17b", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    
    if ((canWrite = sem_open("2ex17b", O_CREAT, 0644, 0)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((index_access = sem_open("3ex17b", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if((x = sem_open("4ex17b", O_CREAT, 0644, 1)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    if((normal = sem_open("5ex17b", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    if((special = sem_open("6ex17b", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    if((vip = sem_open("7ex17b", O_CREAT, 0644, 0)) == SEM_FAILED){
        perror("Error in sem_open()");
        exit(1);
    }
    shared_data->lugares_livres = 2;
    shared_data->atual_na_fila = 1;
    shared_data->index = 1;
    shared_data->quant_vips = 0;
    shared_data->quant_normais = 0;
    shared_data->quant_specials = 0;
    
    //1 - vip
    //2 - special
    //3 - normal
    while(shared_data->lugares_livres > 0){
        sem_wait(canWrite);
        if (shared_data->tipo_cliente == 1){
            shared_data->quant_vips += 1;
        } else if (shared_data->tipo_cliente == 2){
            shared_data->quant_specials += 1;
        } else {
            shared_data->quant_normais += 1;
        }
        shared_data->lugares_livres--;
        printf("Pode entrar\n");
        sem_post(canRead);
    }

    while(1){
        sem_wait(canWrite);
        if (shared_data->tipo_cliente == 1){
            if (shared_data->quant_normais > 0){
                shared_data->quant_normais-=1;
                shared_data->quant_vips+=1;
                sem_post(normal);
            } else if (shared_data->quant_specials > 0){
                shared_data->quant_specials-=1;
                shared_data->quant_vips+=1;
                sem_post(special);
            } else {
                printf("Esta tudo cheio!");
                shared_data->nao_da = 1;
            }
        }
        if (shared_data->tipo_cliente == 2){
            if (shared_data->quant_normais > 0){
                shared_data->quant_normais-=1;
                shared_data->quant_specials+=1;
                sem_post(normal);
            } else {
                printf("Esta tudo cheio!");
                shared_data->nao_da = 1;
            }
        }
        if (shared_data->tipo_cliente == 3){
            printf("Está tudo cheio!");
            shared_data->nao_da = 1;
        }
        sem_post(canRead);
    }
    printf("Acabou\n");

}