#include <stdio.h>
/*
3. Resolver de forma recursiva: Usando aritm�tica de apontadores, implemente 
uma fun��o que recebendo como par�metros o endere�o de um vector de inteiros 
e o seu n�mero de elementos, devolva a soma dos elementos pares do vector.
*/
int sum = 0;

int main(){
	int array[] = {2,5,46,12,1,3,10,7,26,94};
	int numElements = sizeof(array)/sizeof(array[0]);	
	sumEven(array, numElements);
	
	return 0;	
}

void sumEven(int *num, int i){
	i--;
	if(i == -1){											
		printf("Sum of even elements = %d\n", sum);
	}
	if(*(num+i) % 2 == 0){								
		sum += *(num+i);
	}
	sumEven(num, i);										
}










