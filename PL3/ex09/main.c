#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>


#define ARRAY 1000
#define SONS 10



typedef struct{
	int geral[ARRAY];
	int filhos[SONS];
} valores;



int main(void) {
	
	int fd, ft, data_size = sizeof(valores);
    valores *numeros;
    	
	shm_unlink ("/shmtest");
	
	fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1) {
		perror("Erro ao criar memória partilhada!");
		return 1;
	}

	ft = ftruncate (fd, data_size);
	if (ft == -1) {
        perror("Erro na alocação da memória.");
        return 0;
    }
    
	numeros = (valores*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (numeros == MAP_FAILED || numeros == 0) {
        perror("Erro ao mapear a memória.");
        return EXIT_FAILURE;
    }



    int i, j, maximo_geral = 0;
	int status;
    pid_t p[SONS];
    
    srand(time(NULL));
	for (i = 0; i < ARRAY; i++) {
		numeros->geral[i] = rand() % 1000;
		//printf("Número gerado: %d\n", numeros->geral[i]);
	}
			
			
    for (i = 0; i < SONS; i++) {
		
		int maximo_filho = 0;
		
		if ((p[i] = fork()) == -1) {
			perror("Erro na execução do fork.");
		} else if (p[i] == 0) {
			for (j = 100*i; j < (100*i)+99; j++) {
				if (numeros->geral[j] > maximo_filho) {
					maximo_filho = numeros->geral[j];
				}
			}
			numeros->filhos[i] = maximo_filho;
			exit(i);
		}
	}
	
	for (i = 0; i < SONS; i++) {
		waitpid(p[i], &status, 0);
		if (numeros->filhos[i] > maximo_geral) {
			maximo_geral = numeros->filhos[i];
		}
		
	}
	
	printf("Maior valor encontrado: %d \n", maximo_geral);
	
	return 0;
}
