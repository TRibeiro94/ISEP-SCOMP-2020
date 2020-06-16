pointC: pointC.o
	gcc -g -Wall pointC.o -o pointC
	
pointC.o: pointC.c
	gcc -g -Wall -c pointC.c -o pointC.o

clean:
	rm -f *.o pointC

run: pointC
	./pointC
