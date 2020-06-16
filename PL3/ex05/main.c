#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>


typedef struct{
	int numA;
	int numB;
} valores;



int main(void) {
	int fd, i = 0, data_size = sizeof(valores);
	valores *numeros;
	
	shm_unlink ("/shmtest");
	
	fd = shm_open("/shmtest", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		printf("Erro ao criar memória partilhada!");
		return 1;
	}
	
	ftruncate (fd, data_size);
	if (ftruncate (fd, data_size) == -1) {
        perror("Erro na alocação da memória.");
        return 0;
    }
    
	numeros = (valores*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (numeros == MAP_FAILED || numeros == 0) {
        perror("Erro ao mapear a memória.");
        return EXIT_FAILURE;
    }
    
    numeros->numA = 8000;
    numeros->numB = 200;
    
    pid_t p = fork();
    
	if (p < 0) {
			perror("Erro no fork.");
		}
	} else if (p > 0) {
		while (i < 1000000){
			numeros->numA += 1;
			numeros->numB -= 1;
			i++;
		}
	} else if (p == 0) {
		while (i < 1000000){
			numeros->numA -= 1;
			numeros->numB += 1;
			i++;
		}
		exit(0);
    }
    wait(0);
    
    printf("Número A: %d  # # #  Número B: %d\n", numeros->numA, numeros->numB);
    
	return 0;
}

