CC     = gcc
CFLAGS = -Wall
RM     = rm
#------------------------------------------------

# chama o linker
tomasulo:  main.o fila.o util.o
	$(CC) main.o fila.o util.o -o tomasulo

main.o: main.c fila.h util.h
	$(CC) $(CFLAGS) -c main.c 

fila.o: fila.c fila.h
	$(CC) $(CFLAGS) -c fila.c 

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c 

clean: 
	rm -f *.o 