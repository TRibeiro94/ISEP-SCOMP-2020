#include <stdio.h>
#include <unistd.h>	

int main (){
	
    int i;

    for (i = 0; i < 4; i++){
        if (fork () == 0) {
            sleep (1);	
        }
    }
    printf ("This is the end.\n");
    return 0;
}

/*
 * a)
 * Q: How many processes will be created by this code? Justify by drawing a process tree that describes the processes created.
 * A: 2⁴ = 16 (.png at ex6 folder)
 * 
 * b)
 * Q: What change, if any, would you do to this code so that exactly 4 child processes are created?
 * A: Change the '< 4' at the for cycle for '< 2'.
 */
