#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdlib.h>

int main(void){
	
	int num_aleatorio, continua = 1, termina = 0;
	time_t t;
	
	int fd[2];
	int fdt[2];
	int credito = 20;
	
	if(pipe(fd) == -1){ 
		perror("Pipe failed");
        return 1; 
    }
    
    if(pipe(fdt) == -1){ 
		perror("Pipe failed");
        return 1; 
    }
	
	pid_t pid = fork();
	
	if(pid == -1){
        perror("fork failed");
        return 1;
    }
    
    if(pid > 0){
		
		srand((unsigned) time(&t));
		
		close(fd[0]);
		close(fdt[1]);
		
		while(credito > 0){
			num_aleatorio = rand() % 5 + 1;
	
			printf("Número gerado: %d\n", num_aleatorio);
			
			write(fd[1], &continua, sizeof(continua));
			
			int aposta;
			
			read(fdt[0], &aposta, sizeof(int));
			
			if(aposta == num_aleatorio){
				printf("Aposta: %d. Parabéns, ganhou 10 euros na sua conta!\n", aposta);
				credito += 10;
			}
			else{
				printf("Aposta: %d. Ups, perdeu 5 euros.\n", aposta);
				credito -= 5;
			}
			
			write(fd[1], &credito, sizeof(credito));
		}
		
		close(fd[0]);
		
		write(fd[1], &termina, sizeof(termina));
		
		close(fd[1]);
	}
	
	if(pid == 0){
		
		srand((unsigned) time(&t) ^ getpid());
		
		close(fd[1]);
		
		close(fdt[0]);
		
		int credito_atual, notificacao = 1;
		
		while(notificacao == 1){
			
			read(fd[0], &notificacao, sizeof(int));
			
			int aposta = rand() % 5 + 1;
			
			write(fdt[1], &aposta, sizeof(aposta));
			
			read(fd[0], &credito_atual, sizeof(int));
			
			printf("Crédito atual: %d\n", credito_atual);
		}
		
		exit(0);
	}
	
	return 0;
}
