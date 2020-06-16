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
#include <string.h>

#define BUFFER 10
#define OPERATIONS 30



typedef struct {
	int buffer[BUFFER];
    int head;
    int tail;
    int size;
} circular_buffer;



//Exercício 12 do módulo-1
int spawn_childs(int n) {

	int i;

	for (i = 0; i < n; i++) {
		pid_t p = fork();
		if (p == 0) {
			return i+1;
		} else if (p > 0) {
			wait(NULL);
		} else if (p < 0) {
			perror("Erro ao dar o fork.");
			return 0;
		}
	}
	return 0;
}



circular_buffer *start_buffer() {
	int fd, ft, data_size = sizeof(circular_buffer);
	circular_buffer *shared;

	shm_unlink ("/ex13");
	
	fd = shm_open("/ex13", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		perror("Erro ao criar memória partilhada!");
		return 0;
	}

	ft = ftruncate (fd, data_size);
	if (ft == -1) {
        perror("Erro na alocação da memória.");
        return 0;
    }

	shared = (circular_buffer*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (shared == MAP_FAILED || shared == 0) {
        perror("Erro ao mapear a memória.");
        return 0;
    }

    // Defenir o tamanho do buffer
    shared->size = BUFFER;

    // Inicializar os valores da head e tail
    shared->head = 0;
    shared->tail = 0;

    return shared;
}



void write_value_buffer(circular_buffer *shared, int value) {
    // Escrever o valor no buffer
    shared->buffer[shared->head] = value;

//	printf("Foi escrito na posição %d do buffer o valor %d\n", shared->head, value);

    // Avançar com a "cabeça" do buffer uma posição
    shared->head = (shared->head + 1) % shared->size;
}

void read_value_buffer(circular_buffer *shared, int *value) {
    // Ler o valor no buffer
    *value = shared->buffer[shared->tail];

    printf("Foi lida na posição %d do buffer o valor %d\n", shared->tail, *value);

    // Avançar com a "cauda" do buffer uma posição
    shared->tail = (shared->tail + 1) % shared->size;
}



int check_full_buffer(circular_buffer *shared) {
    if ((shared->head + 1) % shared->size == shared->tail) {
        return 1;
    }
    return 0;
}

int check_empty_buffer(circular_buffer *shared) {
    if (shared->head == shared->tail) {
        return 1;
    }
    return 0;
}



int main(void) {

    sem_unlink("semaforo13shm");
    sem_unlink("semaforo13full");
    sem_unlink("semaforo13empty");

    //Semáforo de acesso à memória partilhada
    sem_t *semaforo13shm;
    if ((semaforo13shm = sem_open("semaforo13shm", O_CREAT | O_EXCL, 0644, 1)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    //Semáforo que indica que está cheio o Buffer
    sem_t *semaforo13full;
    if ((semaforo13full = sem_open("semaforo13full", O_CREAT | O_EXCL, 0644, 10)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }
    //Semáforo que indica que está vazio o Buffer
    sem_t *semaforo13empty;	
    if ((semaforo13empty = sem_open("semaforo13empty", O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED){
        perror("Falha na abertura do semáforo");
        exit(1);
    }

	circular_buffer *shared = start_buffer();
	int i, j = 0, k = 0;	

    pid_t p1 = fork();
	if (p1 == -1) {
		printf("Erro no fork.");
	}
    if (p1 == 0) {
		while(j < 3) {
			for (i = 0; i < 10; i++) {
				// Esperar que o buffer não esteja vazio
				sem_wait(semaforo13empty);
				// Adquirir acesso à memória partilhada
				sem_wait(semaforo13shm);
				// Ler valor do buffer
				int value_read;
				read_value_buffer(shared, &value_read);
				// Indicar na flag que leu um valor
				sem_post(semaforo13shm);
				sem_post(semaforo13full);
			}
		j++;
		}
    }
    if (p1 > 0) {
		while(k < 3) {
			for (i = 0; i < 10; i++) {
				// Esperar que o buffer não esteja vazio
				sem_wait(semaforo13full);
				// Adquirir acesso à memória partilhada
				sem_wait(semaforo13shm);
				// Ler valor do buffer
				write_value_buffer(shared, i+1);
				// Indicar na flag que leu um valor
				sem_post(semaforo13shm);
				sem_post(semaforo13empty);
			}
			k++;
		}
        // Esperar pelo processo filho
        wait(NULL);

        if (munmap(shared, 400) < 0) {
            exit(1);
        }
    }
	return 0;
}
