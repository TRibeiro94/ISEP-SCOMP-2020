ex06: main.o
	gcc -g -Wall main.o -o ex06 -lpthread -lrt

main.o: main.c
	gcc -g -Wall -c main.c -o main.o -lpthread -lrt

clean:
	rm -f *.o ex06

run: ex06
	./ex06
