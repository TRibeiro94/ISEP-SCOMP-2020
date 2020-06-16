#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <string.h>

typedef struct {
	char randomChar[100];
} shared_data_type;

int main(void) {
	int fd, data_size = sizeof(shared_data_type);
	shared_data_type *shared_data;
	fd = shm_open("/ex04", O_RDWR, S_IRUSR|S_IWUSR);
	if (fd == -1){
		printf("Erro a abrir a memória partilhada!");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	char temp[100];
	strcpy(temp, shared_data->randomChar);
	for (int i = 0; i < 100; i++){
		printf("%d\n", temp[i]);
	}
	//shared_data->randomChar
	printf("%s ", shared_data->randomChar);
}