#include <stdio.h>
#include <unistd.h>	
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (){
	
	printf("\n\n");
    int i = 1;
    int status;
    pid_t p;

    for (i = 0; i < 2; i++){
		p = fork();
        
        if (p == 0) {
			printf("Sleeping for 1 second.\n");
            sleep (1);
            i += 1;
            exit(i);
        }else{
			if(p % 2 == 0){
				printf("Waiting for process with PID = %d\n", p);
				waitpid(p, &status, 0);
			}	
			waitpid(p, &status, 0);
			if(WIFEXITED(status)){
				printf("Return value = %d (pid = %d)\n", WEXITSTATUS(status), p);
			}
		}
    }
    
    printf("\n");
    return 0;
}
