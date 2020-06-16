#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
	
	int fd[2];
	
	if(pipe(fd) == -1){ 
		perror("Pipe failed");
        return 1; 
    }
	
	pid_t pid = fork();
	
	if(pid == -1){
        perror("fork failed");
        return 1;
    }
	
	int num;
	char str[50];
	
	if(pid > 0){
		close(fd[0]);
		printf("Número: ");
		scanf("%d", &num);
		write(fd[1], &num, sizeof(num));
		printf("String: ");
		scanf("%s", str);
		write(fd[1], str, sizeof(str)+1);
		close(fd[1]);
	}
	
	if(pid == 0){
		close(fd[1]);
		read(fd[0], &num, sizeof(num));
		read(fd[0], str, sizeof(str));
		printf("Processo Filho: o número é %d\n", num);
		printf("Processo Filho: a string é %s\n", str);
		close(fd[0]);
	}
	
	return 0;
}
