CC     = gcc
CFLAGS = -Wall -Wno-unused-result
LDFLAGS = -lm
RM     = rm
#------------------------------------------------

# chama o linker
tomasulo:  main.o tomasulo.o arquivo.o componente.o memoria.o instrucao.o fila.o util.o
	$(CC) main.o tomasulo.o arquivo.o componente.o memoria.o instrucao.o fila.o util.o -o tomasulo $(LDFLAGS)

main.o: main.c tomasulo.h arquivo.h
	$(CC) $(CFLAGS) -c main.c 

tomasulo.o: tomasulo.c tomasulo.h componente.h instrucao.h memoria.h
	$(CC) $(CFLAGS) -c tomasulo.c 

arquivo.o: arquivo.c arquivo.h tomasulo.h instrucao.h memoria.h util.h
	$(CC) $(CFLAGS) -c arquivo.c 

componente.o: componente.c componente.h instrucao.h
	$(CC) $(CFLAGS) -c componente.c 

instrucao.o: instrucao.c instrucao.h tomasulo.h util.h
	$(CC) $(CFLAGS) -c instrucao.c

fila.o: fila.c fila.h
	$(CC) $(CFLAGS) -c fila.c 

memoria.o: memoria.c memoria.h instrucao.h
	$(CC) $(CFLAGS) -c memoria.c 

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c 

clean: 
	rm -f *.o 