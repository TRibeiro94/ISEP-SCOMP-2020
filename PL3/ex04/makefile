escritor: escritor.o
	gcc -g -Wall escritor.o -o escritor -lrt
	
escritor.o: escritor.c
	gcc -g -Wall -c escritor.c -o escritor.o

leitor: leitor.o
	gcc -g -Wall leitor.o -o leitor -lrt
	
leitor.o: leitor.c
	gcc -g -Wall -c leitor.c -o leitor.o
	
clean:
	rm *.o

run: escritor leitor 
	./escritor
	./leitor