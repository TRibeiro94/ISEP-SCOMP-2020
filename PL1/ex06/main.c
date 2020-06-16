#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>	
#include <sys/types.h>
#include <sys/wait.h>

int main (){
	
    int i;
	printf("\n");
    for (i = 0; i < 4; i++){
        if (fork () == 0) {
            printf("(son) pid %d from (parent) pid %d.\n",getpid(), getppid());
            exit(0);	
        }
    }
    for(i = 0; i < 4; i++){
		wait(NULL);
	} 
    
    printf ("This is the end.\n");
    return 0;
}

/*
 * a)
 * Q: How many processes will be created by this code? Justify by drawing a process tree that describes the processes created.
 * A: 2^4 = 16 (.png at ex6 folder)
 * 
 * b)
 * Q: What change, if any, would you do to this code so that exactly 4 child processes are created?
 * A: Exchange sleep(1) for exit(0) inside the for loop, and add a loop below as seen above to wait all child processes to finish.
 */
