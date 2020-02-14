#include <stdio.h>
/*
1. Usando aritmética de apontadores, implemente uma função que recebendo como
parâmetros o endereço de um vector de inteiros e o seu número de elementos,
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
