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
#include <semaphore.h>

typedef struct {
    int ocupado;
	int sentidoAtivo;
    int nrCarrosSentido1;
    int nrCarrosSentido2;
} shared_data_type;

int main(void) {
    sem_t *acessoEx;
    sem_t *sentido1PodePassar;
    sem_t *sentido2PodePassar;
    int fd, r, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex16Alterb", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

    if((acessoEx = sem_open("ex16sem1Altere", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    if((sentido1PodePassar = sem_open("ex16sem2Altere", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    if((sentido2PodePassar = sem_open("ex16sem3Altere", O_CREAT)) == SEM_FAILED){
        printf("erro1");
    }
    
    int meu_sentido = 2;
    sem_wait(acessoEx);
    shared_data->nrCarrosSentido2++;
    if (shared_data->ocupado == 1 && shared_data->sentidoAtivo != meu_sentido){
        sem_post(acessoEx);
        printf("Estou à espera de poder passar a ponte!\n");
        sem_wait(sentido2PodePassar);
    } else {
        if (shared_data->ocupado == 0){
            shared_data->ocupado = 1;
            shared_data->sentidoAtivo = meu_sentido;
        }
        sem_post(acessoEx);
    }
    printf("Estou a cruzar a ponte\n");
    sleep(20);
    sem_wait(acessoEx);
    shared_data->nrCarrosSentido2--;
    if (shared_data->nrCarrosSentido2 == 0){
        if (shared_data->nrCarrosSentido1 != 0){
            shared_data->sentidoAtivo = 1;
        } else {
            shared_data->ocupado = 0;
            shared_data->sentidoAtivo = 0;
        }
        int i;
        for(i = 0; i < shared_data->nrCarrosSentido1; i++){
            sem_post(sentido1PodePassar);
        }
    }
    sem_post(acessoEx);

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
}