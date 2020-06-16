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
#include <time.h>

typedef struct {
	char frase[80];
} info;

typedef struct{
    info array[50];
    int arraypos;
} shared_data_type;


int main(void) {
	int fd, r, i, data_size = sizeof(shared_data_type);
    sem_t *sem;
    struct timespec time;
    if (clock_gettime(CLOCK_REALTIME, &time)){
        printf("Erro no clock!");
        return 1;
    }
    time.tv_sec += 12;
    if ((sem = sem_open("sema", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
        sem = sem_open("sema", O_CREAT);
    }
	shared_data_type *shared_data;
	fd = shm_open("/pl4ex04", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd == -1){
		printf("Erro na criação da memória partilhada!\n");
		return 1;
	}
	if (ftruncate (fd, data_size) == -1){
        printf("Erro no ftruncate!\n");
        return 1;
    }
	shared_data = (shared_data_type*)mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if (sem_timedwait(sem, &time) == -1){
        printf("Não foi possivel aceder à memória!\n");
        return 1;
    }
    printf("Add(1) or Remove(0) ?\n");
    scanf("%d", &i);
    if (i == 1){
        int pos = shared_data->arraypos;
        char temp[80];
        snprintf(temp, sizeof(temp), "I'm the father - with PID %d", getpid());
        strcpy(shared_data->array[pos].frase, temp);
        shared_data->arraypos = pos+1;
    }
    if (i == 0){
        int x;
        printf("Qual quer dar free?\n");
        scanf("%d", &x);
        int pos = shared_data->arraypos;
        if (x >= 0 && x < pos){
            strcpy(shared_data->array[x].frase, "");
            printf("Sucesso!\n");
        } else {
            printf("Posição invalida!\n");
        }
    }
    sem_post(sem);

    r = munmap(shared_data, data_size);
    if (r == -1){
        exit(1);
    }
    r = close(fd);
    if (r == -1){
        exit(1);
    }
}