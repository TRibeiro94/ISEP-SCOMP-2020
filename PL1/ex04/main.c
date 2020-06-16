#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>	   

int main(void) {
	int a = 0, b, c, d;
	b = (int) fork();
	c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
	d = (int) getppid();
	a = a + 5;
	if(b > 0){
		printf("\nF a=%d, b=%d, c=%d, d=%d\n",a ,b ,c ,d);
	}else{
		printf("\nP a=%d, b=%d, c=%d, d=%d\n",a ,b ,c ,d);
	}
	
	
	return 0;
}

/*
 * a) 
 * Q: Which of the variables a, b, c and d will have the same value in both processes?
 * A: Only variable 'a'.
 * 
 * b) 
 * Q: Draw a process tree that describes the processes created.
 * A: (.png in ex4 folder)
 */
