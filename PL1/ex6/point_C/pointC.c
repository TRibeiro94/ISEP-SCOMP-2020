#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>

int main (){
	
    int i, status;
    pid_t arrayPids[4];

    for (i = 0; i < 4; i++){
        if((arrayPids[i] = fork ()) == 0) {
			printf("(son) pid %d from (parent) pid %d.\n", getpid(), getppid());
            exit(0);	
        }
    }
    for(i = 0; i < 4; i++){
		if(arrayPids[i] % 2 == 0){
			printf("Waiting for child process with even PID (%d)\n", arrayPids[i]);
			waitpid(arrayPids[i], &status, 0);
		}
	}
    
    printf ("This is the end.\n");
    return 0;
}
