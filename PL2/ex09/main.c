#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define ARRAY 50000
#define CHILD 10

struct sales {
    int customer_code;
    int product_code;
    int quantity;
};

int main() {
    int fd[2], products[ARRAY], i, j, status, code;
    struct sales array[ARRAY];
    pid_t pids[CHILD];
	size_t bytes_read;

	//randomização numérica de modo a preencher a estrutura
    srand(time(NULL));			//para prevenir sequências repetidas
	for (i = 0; i < ARRAY; i++) {
        array[i].customer_code = rand() % 50;
        array[i].product_code = rand() % 20;
        array[i].quantity = rand() % 40;
    }

    if (pipe(fd) == -1) {
        perror("Erro");
        return 0;
    }

    //serão dados 10 forks de modo a criar 10 Filhos e o PID de cada filho será armazenado num array
    //em cada fork, dentro do processo Filho, vamos verificar se o valor da quantidade é inferior a 20
	//num total de 50.000, cada Filho irá processar cada 5.000, daí o incremento do +5000
	for (i = 0; i < CHILD; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
			close(fd[0]);
			for (j = 5000 * i; j < (5000 * i) + 5000; j++) {
				if (array[j].quantity > 20) {
					write(fd[1], &array[j].product_code, sizeof(array[j].product_code));
				}
			}
			exit(0);
		}
    }
    close(fd[1]);
    //enquanto há algo para ser lido o Array de Produtos é incrementado
    while ((bytes_read = read(fd[0], &code, sizeof(code))) > 0) {
        products[i] = code;
        i++;
    }

	//o processo Pai espera que todos os Filhos terminem
    for (i = 0; i < CHILD; i++) {
        waitpid(pids[i], &status, 0);
    }

	//mostra o código dos produtos que venderam mais do que 20 unidades
    for (i = 0; i < ARRAY; i++) {
        if (products[i] > 0) {
            printf("Código de produto: %d\n", products[i]);
        }
    }

    return 0;
}
