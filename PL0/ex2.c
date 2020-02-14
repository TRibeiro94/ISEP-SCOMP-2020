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
	
	printf("Insert a value to subtract to all elements:\n");
	scanf("%d", &value);
	subValueToAllElements(array, length, value);
	
	printf("[ ");								//prints the inicial square bracket of the array
	for(j = 0; j < length; j++){
		if(j == length-1){					    //if it's the last element, prints it and closes it with the square bracket
			printf("%d ]", *(array+j));	
		}else{
			printf("%d, ", *(array+j));			//if not the last, just print the element and a comma
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
