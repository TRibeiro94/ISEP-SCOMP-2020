shm: shm.o
	gcc -g -Wall shm.o -o shm -lrt
	
shm.o: shm.c
	gcc -g -Wall -c shm.c -o shm.o

pipes: pipes.o
	gcc -g -Wall pipes.o -o pipes -lrt
	
pipes.o: pipes.c
	gcc -g -Wall -c pipes.c -o pipes.o
	
clean:
	rm *.o

run: shm pipes
	./shm
	./pipes
