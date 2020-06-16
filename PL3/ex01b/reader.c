#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


typedef struct{
	int number;
	char name[100];
}student;



int main(void) {
	int fd, data_size = sizeof(student);
	student *aluno;
	
	fd = shm_open("/shmtest", O_RDWR, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		return fd;
	}

	ftruncate (fd, data_size);
	aluno = (student *) mmap(NULL, data_size, PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	if(aluno == NULL) {
		return -1;
	}
	printf("Os dados introduzidos no Writer foram os seguintes:\n");
	printf("%d \n", aluno->number);
	printf("%s \n", aluno->name);

	return 0;
}
