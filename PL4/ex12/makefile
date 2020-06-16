main: main.o
	gcc -g -Wall main.o -o main -lrt -lpthread
	
main.o: main.c
	gcc -g -Wall -c main.c -o main.o
	
clean:
	rm *.o

run: main
	./main