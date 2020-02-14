#include <stdio.h>
/*
4. Resolver recursivamente: Usando aritmética de apontadores, implemente uma função 
que recebendo como parâmetros o endereço de um vector de inteiros, o seu número de 
elementos e um determinado valor, subtraia esse valor a todos os elementos do vector.
*/
int array[] = {10,15,20,25,30,35,40,45,50,55};
int num = sizeof(array)/sizeof(array[0]);

int main(){
	int target;
	
	printf("Insira uma valor para ser subtraido a todos os elementos:\n");
	scanf("%d", &target);
	subValueToAllElements(array, num, target);
	
	return 0;
}

void subValueToAllElements(int *array, int numElements, int target){
	int j;
	numElements--;				
	
	if(numElements == -1){
		printf("[ ");
		for(j = 0; j < num; j++){
			if(j == num - 1){								
				printf("%d ]", *(array+j));	
			}else{
				printf("%d, ", *(array+j));
			}
		}
    }else{
    	*(array+numElements) -= target;
	}
	subValueToAllElements(array, numElements, target);												
}



