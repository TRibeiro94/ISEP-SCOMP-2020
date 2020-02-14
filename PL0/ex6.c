#include <stdio.h>
#include <string.h>
/*
6. Usando aritm�tica de apontadores, implemente uma fun��o que recebendo como par�metro 
o endere�o de duas strings, copia o conte�do da primeira string para a segunda string.
*/
int main(){
	char string1[] = "Varok Saurfang";
	int length = strlen(string1);
	char string2[length];

	copyString(string1, string2);
	printf("This is string 1: %s\n", string1);
	printf("This is string 2: %s\n", string2);
	
	return 0;
}

//this is the iterative method
void copyString(char *string1, char *string2){
	int i;
	for(i = 0; i < strlen(string1); i++){
		*(string2 + i) = *(string1 + i);
	}
}

//this is the straight foward method, does the same thing as above
void copyString2(char *string1, char *string2){
	strcpy(string2, string1);
}



