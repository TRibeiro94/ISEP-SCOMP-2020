#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/wait.h>

typedef struct {
	int value;
} shared_data_type;

int main(void) {
	int fd, i = 0, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex08", O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		printf("Erro na criação da memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    pid_t p = fork();
    if (p == -1){
        printf("Erro no fork!");
        return 1;
    }
    shared_data->value = 100;
    if (p > 0){
        while (i < 1000000){
            shared_data->value += 1;
            shared_data->value -= 1;
            i++;
        }
    } else {
        while (i < 1000000){
            shared_data->value += 1;
            shared_data->value -= 1;
            i++;
        }
        exit(0);
    }
    wait(NULL);
	printf("%d\n", shared_data->value);

}
//A maior parte das vezes termina com 100, mas algumas vezes termina em 99.