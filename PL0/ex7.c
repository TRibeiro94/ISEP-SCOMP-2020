#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
7. Implemente uma função que seja capaz de identificar se duas strings podem ser
lidas da mesma forma de trás para a frente e de frente para trás. (palindrome).
## Resolvido com memoria dinamica ##
*/

int main(){
	char *string1, *string2;
	int n1, n2;
	
	// FIRST STRING
	printf("Insira quantos carateres vai ter no maximo a string 1:\n");

	scanf("%d", &n1);
	string1 = malloc(n1+1);													//n1 bytes + 1 para o '\0' no fim de uma string
	if(string1 == NULL){
		printf("\nFalha ao alocar memoria para %d bytes.", n1+1);
		exit(EXIT_FAILURE);													//exit se não for possivel alocar memoria
	}
	
	printf("\nInsira a string 1 (maximo %d carateres.)\n", n1);
	getchar();																//limpar o buffer
	scanf("%[^\n]s", string1);												//ler a string com espacos
	
	
	// SECOND STRING
	printf("\nInsira quantos carateres vai ter no maximo a string 2:\n");
	scanf("%d", &n2);
	string2 = malloc(n2+1);
	if(string2 == NULL){
		printf("\nFalha ao alocar memoria para %d bytes.", n2+1);
		exit(EXIT_FAILURE);
	}
	printf("\nInsira a string 2 (maximo %d carateres.)\n", n2);
	getchar();
	scanf("%[^\n]s", string2);
	
	
	if(palindrome(string1) == 1 && palindrome(string2) == 1){
		printf("Ambas as strings sao palindromes.");
	}else if(palindrome(string1) == 0 && palindrome(string2) == 1 || palindrome(string1) == 1 && palindrome(string2) == 0){
		printf("Uma string palindrome, outra nao.");
	}else{
		printf("Nenhuma das strings sao palindromes.");
	}
	
	free(string1);
	free(string2);
	
	return 0;
}


int palindrome(char *str){
	int i = 0, length = strlen(str) - 1;
	if(str[0] == '\0'){					
		return 0;
	}

	while(length > i){
		if(str[i]==',' || str[i]==' ' || str[i]=='.'){
			i++;
		}else if(str[length]==',' || str[length]==' ' || str[length]=='.'){
			length--;
		}else if(toupper(str[i]) != toupper(str[length])){
			return 0;
		}else{
			i++;
			length--;	
		}
	}
	return 1;
}
