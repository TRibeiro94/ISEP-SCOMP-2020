#include <stdio.h>
/*
2. Usando aritmética de apontadores, implemente uma função que recebendo como
parâmetros o endereço de um vector de inteiros, o seu número de elementos e um
determinado valor, subtraia esse valor a todos os elementos do vector.
*/
int main(){
	
	int array[] = {23,5,46,12,1,3,10,7,26,94};
	int length = sizeof(array)/sizeof(array[0]);
	int *ptr = array;
	int value, j;
	
	printf("Insira um valor para subtrair a todos os elementos:\n");
	scanf("%d", &value);
	subValueToAllElements(array, length, value);
	
	printf("[ ");								
	for(j = 0; j < length; j++){
		if(j == length-1){					
			printf("%d ]", *(array+j));	
		}else{
			printf("%d, ", *(array+j));			
		}
	}
	return 0;	
}

void subValueToAllElements(int *num, int length, int value){
	int i;
	
	for(i = 0; i < length; i++){
		*(num+i) -= value;
	}
}
