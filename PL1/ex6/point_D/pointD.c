#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>

int main (){
	
    int i, j, status;
    pid_t arrayPids[4];

    for (i = 0; i < 4; i++){
        if((arrayPids[i] = fork ()) == 0) {
			printf("(son) pid %d from (parent) pid %d.\n", getpid(), getppid());
            exit(i++);	
        }
    }
    for(j = 0; j < 4; j++){
		if(arrayPids[j] % 2 == 0){
			printf("Waiting for child process with even PID (%d)\n", arrayPids[j]);
			waitpid(arrayPids[j], &status, 0);
			if(WIFEXITED(status)){
				printf("Even PID %d was executed successfully. Order : %d\n",arrayPids[j], WEXITSTATUS(status));
			}
		}
	}
    
    printf ("Parent process.\n");
    return 0;
}
