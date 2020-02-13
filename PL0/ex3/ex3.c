#include <stdio.h>
/*
3. Resolver de forma recursiva: Usando aritmética de apontadores, implemente 
uma função que recebendo como parâmetros o endereço de um vector de inteiros 
e o seu número de elementos, devolva a soma dos elementos pares do vector.
*/
int main(){
	
	int array[] = {23,5,46,12,1,3,10,7,26,94};
	int numElements = sizeof(array)/sizeof(array[0]);
	int *ptr = array;
	int sum = 0, i = 0;

	sumEven(array, numElements, sum, i);
	return 0;	
}

void sumEven(int *num, int numElements, int sum, int i){
	
	if(numElements == 0){										//if no more elements to iterate, print the sum value
		printf("Sum of even elements = %d\n", sum);
	}
	if(*(num+i) % 2 == 0){										//if element is pair, add it to sum variable
		sum += *(num+i);
	}															//recursive call, passing (array pointer, number of elements minus one, 
	sumEven(num, numElements-1, sum, i+1);						//the current sum value, and the current i value being used to iterate the array		
}








