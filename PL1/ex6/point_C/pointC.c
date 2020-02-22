#include <stdio.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>

int main (){
	
    int i;
    int status;
    pid_t p;

    for (i = 0; i < 2; i++){
		p = fork();
        
        if (p == 0) {
			printf("Sleeping for 1 second.\n");
            sleep (1);	
        }else{
			if(p % 2 == 0){
				printf("Waiting for child process with even PID = %d\n", p);
				waitpid(p, &status, 0);
			}	
		}
    }
    
    printf ("This is the end.\n");
    return 0;
}
