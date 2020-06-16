#include <stdio.h>
#include <sys/types.h> //pid_t
#include <unistd.h>	   //fork()

int main(){
	
	int x = 1;
	pid_t p;
	p = fork();
	
	// parent & son processes are running concurrently
	// parent's pid is always > 0
	// son's pid is always = 0
	
	if(p == 0){
		x += 1;
		printf("1. x = %d\n", x);
	}else{
		x -= 1;
		printf("2. x = %d\n", x);
	}
	printf("3. %d; x = %d\n", p, x);
	
	return 0;
}

/* a)
	Q: Assume that the PID of the child process is 1234. What is the output of this code in the terminal? Justify your answer.
	A: 2. x = 0
	   3. PID_VALUE; x = 0
	   1. x = 2
	   3. 0; x = 2 (0 because it's the son process now)
	
   b)
	Q: Are you always guaranteed to see the output of the printf() in line 9 before the one in line 11? Explain.
	A: No. There are no guarantees after a fork because it depends on the order the scheduler runs the processes in.
	     Yet, in this case, with the if/else statement as it is, we can 'force' the program to run the 'else' block of code first, aswel
	     as the last 'printf' instruction, and only then, it will run the 'if' block of code, and the last 'printf' one final time.
*/
