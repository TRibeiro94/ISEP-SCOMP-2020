#include <stdio.h>
/*
5. Usando aritmética de apontadores, implemente uma função que recebendo como parâmetros o 
endereço de um vector de inteiros, o seu número de elementos, coloque num segundo vector 
(o terceiro parâmetro da função) o inverso de cada um dos valores do primeiro vector.
*/
int main(){
	int original[] = {1, 2, 3, 4, 5};
	int numElements = sizeof(original)/sizeof(original[0]);
	int copia[numElements], j;
	
	invertElements(original, numElements, copia);
	
	printf("[ ");
	for(j = 0; j < numElements; j++){
		if(j == numElements - 1){								
			printf("%d ]", *(copia+j));	
		}else{
			printf("%d, ", *(copia+j));
		}
	}
	
	return 0;
}

void invertElements(int *array1, int numElements, int *array2){
	int i;
	
	for(i = 0; i < numElements; i++){
		*(array2 + i) = *(array1 + i) * (-1);
	}
}
