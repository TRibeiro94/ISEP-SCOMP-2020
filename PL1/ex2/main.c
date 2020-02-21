#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
	
	printf("I'm..\n");
	pid_t p1 = fork();
	wait(NULL);							//p1 parent process waits for son to execute 'printf' at line 27, 
										//and then enters the 'if' at line 12
	if(p1 > 0) {
		pid_t p2 = fork();
		if(p2 > 0) {
			printf("the..\n");
			wait(NULL);					//p2 parent process waits for son to execute 'printf' at line 24
			pid_t p3 = fork();			//and then executes the 3rd fork
			if(p3 > 0){					//p3 executes normally the parent process 1st, and then the son process
				printf("father!\n");	
			}else{
				printf("I'll never join you! (3rd)\n");	
			}
		}else{
			printf("I'll never join you! (2nd)\n");	
		}
	}else{
		printf("I'll never join you! (1st)\n");	
	}
	return 0;
}
