#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/time.h>

typedef struct {
	int code;
	char description[50];
	int price;
	int canRead;
} product_info;

int main(void){
	int fd, r, data_size = sizeof(product_info);
	product_info *product;
	
	fd = shm_open("/ex2", O_RDWR, S_IWUSR|S_IRUSR);
	if (fd == -1) exit(1);
	
	int aux = ftruncate(fd, data_size);
	if (aux == -1) exit(1);
	
	product = (product_info *) mmap(NULL, data_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (product == MAP_FAILED) exit(1);
	
	printf("\nREADER\n");
	
	printf("Code: %d\n", product->code);
	
	printf("Description: %s\n", product->description);
	
	printf("Price: %d\n", product->price);
	
	r = munmap(product, data_size);
	if (r < 0) exit(1);
	
	r = shm_unlink("/ex2");
	if (r < 0) exit(1);
	
	return 0;
}
