program: insert consult consult_all

insert:  insert.o
		gcc -Wall -g  insert.o -o insert -lrt -lpthread

insert.o: insert.c
		gcc -Wall -g -c insert.c -o insert.o -lrt -lpthread

consult:  consult.o
		gcc -Wall -g  consult.o -o consult -lrt -lpthread

consult.o: consult.c
		gcc -Wall -g -c consult.c -o consult.o -lrt -lpthread

consult_all:  consult_all.o
		gcc -Wall -g  consult_all.o -o consult_all -lrt -lpthread

consult_all.o: consult_all.c
		gcc -Wall -g -c consult_all.c -o consult_all.o -lrt -lpthread

clean:
		rm *.o
