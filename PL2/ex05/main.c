#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//tamanho pré-definido a ser usado pela mensagem
#define BUFFER_SIZE 256

//processamento da mensagem
void conv_string(char *string) {
    while (*string) {
        if (*string >= 'A' && *string <= 'Z') {
            *string = *string + 32;
        }
        else if (*string >= 'a' && *string <= 'z') {
            *string = *string - 32;
        }
        string++;
    }
}

int main() {

    int fd1[2], fd2[2];
    int status;

    char stdin[BUFFER_SIZE];
    char stdout[BUFFER_SIZE];
    
    if((pipe(fd1) == -1) || (pipe(fd2) == -1)) {
		perror("Erro");
		return 0;
	}

    if (fork() > 0) {
		close(fd1[1]);
		read(fd1[0], stdout, BUFFER_SIZE);
		close(fd1[0]);

		conv_string(stdout);
        
		close(fd2[0]);
		write(fd2[1], stdout, strlen(stdout) + 1);
		close(fd2[1]);

		waitpid(fork(), &status, 0);
    } else if (fork() == 0) {
		close(fd1[0]);

		printf("Escreva uma expressão:\n");
		scanf("%[^\n]s", stdin);

		write(fd1[1], stdin, strlen(stdin) + 1);
		close(fd1[1]);

		close(fd2[1]);
		read(fd2[0], stdin, BUFFER_SIZE);
		close(fd2[0]);

		printf("Nova palavra: %s\n", stdin);
        
		exit(0);
    }

    return 0;
}
