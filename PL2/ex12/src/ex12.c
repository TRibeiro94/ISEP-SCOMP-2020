/*
 ============================================================================
 Name        : ex12.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

typedef struct{
	int id;
	int cost;
	char name[10];
} product;

product* verificaInventario(int id, product* inventario){
	int i;
	for(i = 0; i < 5; i++){
		if(inventario[i].id == id){
			return &inventario[i];
		}
	}
	return NULL;
}

int main(void) {
	int i, j;

	product inventario[5];
	inventario[0].id = 1;
	strcpy(inventario[0].name, "Iogurte");
	inventario[0].cost = 2;
	inventario[1].id = 2;
	strcpy(inventario[1].name, "Massa");
	inventario[1].cost = 4;
	inventario[2].id = 3;
	strcpy(inventario[2].name, "Atum");
	inventario[2].cost = 6;
	inventario[3].id = 4;
	strcpy(inventario[3].name, "Papel");
	inventario[3].cost = 8;
	inventario[4].id = 5;
	strcpy(inventario[4].name, "Carne");
	inventario[4].cost = 10;
	pid_t arrayPids[5];
	int fd[12];

	pipe(fd);

	for(i = 0; i < 5; i++){
			pipe(&fd[(i * 2) + 2]);
			arrayPids[i] = fork();

			if (arrayPids[i] == -1){
				printf("Error!\n");
				exit(1);
			}

			if(arrayPids[i] == 0){
				close(fd[0]);
				for (j = 0; j <= i; j++){
					close(fd[(j * 2) + 3]);
				}
				int cost, id = i+1, x = (i*2) + 3;
				char name[10];
				write(fd[1], &id, sizeof(id));
				write(fd[1], &x, sizeof(x));
				read(fd[(i * 2) + 2], &name, sizeof(name));
				read(fd[(i * 2) + 2], &cost, sizeof(cost));
				printf("Name: %s - Cost: %d\n", name, cost);
				exit(0);
			}
	}
	close(fd[1]);
	int id, x, k = 0;

	while(k < 5){
		read(fd[0], &id, sizeof(id));
		read(fd[0], &x, sizeof(x));
		//printf("%d\n", id);
		product* a;
		a = verificaInventario(id, inventario);
		//printf("%d - %d - %s\n", a->id, a->cost, a->name);
		write(fd[x], a->name, sizeof(a->name));
		int co = a->cost;
		write(fd[x], &co, sizeof(co));
		k++;
	}

	for(i = 0; i < 5; i++){
		wait(NULL);
	}
}


