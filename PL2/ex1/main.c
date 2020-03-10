#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>

int main(){
	
	pid_t p;
	int written, bytesRead;
	int table[2];
	pipe(table);
	p = fork();
	
	if (p > 0){
		close(table[0]);
		written = getpid();
		printf("\nNº bytes written: %d\n", sizeof(written));
		printf("PID written: %d\n--------------------\n", written);
		write(table[1],&written, sizeof(int));
		close(table[1]);
	}else{
		close(table[1]);
		bytesRead = read(table[0],&written , sizeof(int));
		printf("Nº bytes read: %d\n", bytesRead);
		printf("PID read: %d\n\n", written);
		close(table[0]);
	}
	
	return 0;
}
