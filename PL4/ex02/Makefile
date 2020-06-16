ex02: main.o
	gcc -g -Wall main.o -o ex02 -lpthread -lrt

main.o: main.c
	gcc -g -Wall -c main.c -o main.o -lpthread -lrt

clean:
	rm -f *.o ex02

run: ex02
	./ex02
