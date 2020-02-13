#include <stdio.h>
/*
1. Usando aritm�tica de apontadores, implemente uma fun��o que recebendo como
par�metros o endere�o de um vector de inteiros e o seu n�mero de elementos,
devolva a soma dos elementos pares do vector.
*/
int main(){
	
	int array[] = {23,5,46,12,1,3,10,7,26,94};
	int length = sizeof(array)/sizeof(array[0]);
	int *ptr = array;
	
	printf("Soma dos elementos pares = %d\n", soma(ptr, length));
	return 0;	
}

int soma(int *num, int length){
	int i, soma = 0;
	
	for(i = 0; i < length; i++){
		if(*(num+i) % 2 == 0){
			soma += *(num+i);
		}
    }
	
	return soma;
}
