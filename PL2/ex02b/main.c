#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void){
	
	int fd[2];
	int numero;
	char string[50];
	
	struct s1 {
        int num;
        char str[50];
    };
	
	if(pipe(fd) == -1){ 
		perror("Pipe failed");
        return 1; 
    }
	
	pid_t pid = fork();
	
	if(pid == -1){
        perror("fork failed");
        return 1;
    }
	
	if(pid > 0){
		close(fd[0]);
		printf("Número: ");
		scanf("%d", &numero);
		printf("String: ");
		scanf("%s", string);
		struct s1 s_pai;
		s_pai.num = numero;
		strcpy(s_pai.str, string);
		write(fd[1], &s_pai, sizeof(s_pai));
		close(fd[1]);
	}
	
	if(pid == 0){
		close(fd[1]);
		struct s1 s_filho;
		read(fd[0], &s_filho, sizeof(s_filho));
		printf("Processo Filho: o número é %d\n", s_filho.num);
		printf("Processo Filho: a string é %s\n", s_filho.str);
		close(fd[0]);
	}
	
	return 0;
}
