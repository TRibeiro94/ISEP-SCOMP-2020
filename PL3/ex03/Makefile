#vars
INCLUDES =
SOURCES = main.c
OBJFILES = main.o
EXEC = main
FLAGS = -Wall -g
LIBS =  -lrt
# regras de sufixo
.SUFIXES : .c .o

# como transformar um .c num .o ; $< --nome do ficheiro
.c.o:
	gcc $(FLAGS) -c $< $(LIBS)


${EXEC} : ${OBJFILES}
	gcc $(FLAGS) -o ${EXEC} ${OBJFILES} $(LIBS)

${OBJFILES} : ${SOURCES} ${INCLUDES} $(LIBS)

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}

vg:
	valgrind ./${EXEC}

debug:
	gdb -tui ${EXEC}
