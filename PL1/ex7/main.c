#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <unistd.h>	 
#define ARRAY_SIZE 1000

int main(){
	int numbers[ARRAY_SIZE];			//array to lookup
	int n;								//the number to find
	int counter = 0, status, i;
	int half = ARRAY_SIZE / 2;
	time_t t;							//needed to initialize random number generator (RNG)
	pid_t p;							
	srand ((unsigned) time (&t)); 		//intializes RNG (srand():stdlib.h; time(): time.h)

	for (i = 0; i < ARRAY_SIZE; i++){	//initialize array with random numbers (rand(): stdlib.h)
		numbers[i] = rand() % 10000;
	}
	n = rand() % 10000;					//initialize n
	
	p = fork();
	waitpid(p, &status, 0);				//wait for son process
	
	if(p > 0){
		for(i = 0; i < half; i++){
			if (numbers[i] == n){
				counter++;
			}
		}
		printf("'%d' was found: %d times by parent process.\n", n, counter);
		
		if(WIFEXITED(status)){
			int total = WEXITSTATUS(status) + counter;		//exit value of son = times n was found at son process
			printf("Total times found = %d.\n", total);
		}
		
	}else{
		for(i = half; i < ARRAY_SIZE; i++){
			if (numbers[i] == n){
				counter++;
			}
		}
		printf("\n'%d' was found: %d times by son process.\n", n, counter);
		exit(counter);
	}
	return 0;
}
