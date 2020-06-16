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

#define STR_SIZE 50
#define NR_DISC 10

typedef struct {
	int numero;
    char nome[STR_SIZE];
    int disciplinas[NR_DISC];
    int pode_ler;
} shared_data_type;

int main(void) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex12", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	int err = ftruncate (fd, data_size);
    if (err == -1){
        printf("Erro no truncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    shared_data->pode_ler = 0;
    pid_t p = fork();
    if (p == -1){
        printf("Erro no fork!");
        return 1;
    }
    if (p > 0){
        shared_data->numero = 50;
        strcpy(shared_data->nome, "Bruno Miguel");
        for (int i = 0; i < NR_DISC; i++){
            shared_data->disciplinas[i] = (2*i);
        }
        shared_data->pode_ler = 1;
    } else {
        while (!shared_data->pode_ler);
        int temp[NR_DISC];
        for(int i = 0; i < NR_DISC; i++){
            temp[i] = shared_data->disciplinas[i];
        }
        int soma = temp[0];
        int maior = temp[0];
        int menor = temp[0];
        for(int i = 1; i < NR_DISC; i++){
            int valor = temp[i];
            soma += valor;
            if (valor > maior){
                maior = valor;
            }
            if (valor < menor){
                menor = valor;
            }
        }
        int media = (soma/NR_DISC);
        printf("\nEsta é a média: %d", media);
        printf("\nEsta é a maior nota: %d", maior);
        printf("\nEsta é a menor nota: %d\n", menor);
        if (munmap(shared_data, data_size)){
            printf("Erro no munmap!");
            return 1;
        }

        if (close(fd) == -1){
            printf("Erro no close!");
            return 1;
        }
        exit(0);
    }
    wait(NULL);
    if (munmap(shared_data, data_size)){
        printf("Erro no munmap!");
        return 1;
    }

    if (close(fd) == -1){
        printf("Erro no close!");
        return 1;
    }
    
    if (shm_unlink("/ex12")){
        printf("Erro no shm_unlink!");
        return 1;
    }
    return 0;
}