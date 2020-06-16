#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main(){
	pid_t p;
	
	if (fork() == 0) {
		printf("1-PID = %d\n", getpid());
		exit(0);
	}
 
	if ((p=fork()) == 0) {
		printf("2-PID = %d\n", getpid());
		exit(0);
	} 

	printf("Parent PID = %d\n", getpid());
 
	printf("Waiting... (for PID=%d)\n",p); 
	waitpid(p, NULL, 0);
 
	printf("Enter Loop...\n"); 
	while (1); /* Infinite loop */
	
	return 0;
	
/*
 * a)
 * Q: In the shell, list the running processes with ps (look for the PIDs printed). Are all parent and child processes listed? Why?
 * A: No. The second child doesn't appear because it finished its execution and the parent handled its information, leaving the child 1 as zombie and being himself on an infinite loop.
 * 
 * b)
 * Q: Is there anything particular about the child process? Explain.
 * A: Yes. The process it's in zombie state permanently since the parent is on an infite loop, the child finishes and the parent is still
 * executing, not having processed the child information.
 */
}


