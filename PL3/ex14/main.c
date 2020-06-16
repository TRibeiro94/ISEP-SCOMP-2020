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

#define BUFFER 10
#define OPERATIONS 30



typedef struct {
	int buffer[BUFFER];
    int head;
    int tail;
    int size;
} circular_buffer;



circular_buffer *start_buffer() {
	int fd, ft, data_size = sizeof(circular_buffer);
	circular_buffer *shared;

	shm_unlink ("/ex14");
	
	fd = shm_open("/ex14", O_CREAT|O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
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
	
	int i;
	
	circular_buffer *shared = start_buffer();
    
    pid_t p = fork();
	if (p == -1) {
		printf("Erro no fork.");
	}
	
    if (p == 0) {
        for (i = 0; i < OPERATIONS; i++) {
            while (check_empty_buffer(shared)) {
                printf("O buffer está vazio. Nada pode ser lido.\n");
                sleep(1);
            }
            int value_read;
            read_value_buffer(shared, &value_read);
        }
        exit(0);
    } else if (p > 0) {
        for (i = 0; i < OPERATIONS; i++) {
            while (check_full_buffer(shared)) {
                printf("O buffer está cheio. Nada pode ser escrito.\n");
                sleep(1);
            }
            write_value_buffer(shared, i);
        }
        
        // Esperar pelo processo filho
        wait(NULL);
        
        if (munmap(shared, 150) < 0) {
            exit(1);
        }
        if (shm_unlink("/ex14") < 0) {
            exit(1);
        }
    }
	
	return 0;
}
