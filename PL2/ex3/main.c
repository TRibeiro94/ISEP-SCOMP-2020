#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>
#include <sys/wait.h>
#include <string.h>

int main(){

	pid_t p, childPid;
	int read1, read2, status;
	char msg1[] = "Hello World";
	char msg2[] = "Goodbye!";
	int sizeString1 = strlen(msg1) + 1;
	int sizeString2 = strlen(msg2) + 1;
	int table[2];
	pipe(table);
	p = fork();
	
	if (p < 0) {
        printf("Error on fork system call.");
		exit(1);
    }else if(p > 0){
		close(table[0]);		//close reading on parent
		printf("\nNº bytes in 1st string: %d", sizeof(msg1));
		printf("\nNº bytes in 2nd string: %d\n\n", sizeof(msg2));
		printf("1st message to be written: %s\n", msg1);
		printf("2nd message to be written: %s\n--------------------\n", msg2);
		write(table[1], msg1, sizeof(msg1));
		write(table[1], msg2, sizeof(msg2));
		close(table[1]);		//close writting on parent
	}else{
		close(table[1]);		//close writting on son
		char str1[sizeString1];	//array with string 1 size to store what was read
		char str2[sizeString2];	//  "     "  string 2  "    "    "    "   "    "
		read1 = read(table[0], str1, sizeof(msg1));
		read2 = read(table[0], str2, sizeof(msg2));
		printf("\nNº bytes read from 1st string: %d", read1);
		printf("\nNº bytes read from 2nd string: %d\n", read2);
		close(table[0]);		//close reading on son
		exit(0);
	}

    childPid = wait(&status);
    printf("\nChild PID [%d] exited with value = %d.\n\n", childPid, WEXITSTATUS(status));

	return 0;
}
