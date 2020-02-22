pointD: pointD.o
	gcc -g -Wall pointD.o -o pointD
	
pointD.o: pointD.c
	gcc -g -Wall -c pointD.c -o pointD.o

clean:
	rm -f *.o pointD

run: pointD
	./pointD