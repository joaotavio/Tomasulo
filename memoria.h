#ifndef MEMORIA_H
#define MEMORIA_H 

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "instrucao.h"

extern int tam_memoria;
extern uint8_t* memoria;

void inicializaMemoria(int tam_memoria);
void memoriaInsereInst(Instrucao inst, int posicao);
void memoriaInsereDado(int valor, int posicao);
Instrucao memoriaObterInst(int posicao);
int memoriaObterDado(int posicao);
void printMemoria();

#endif
