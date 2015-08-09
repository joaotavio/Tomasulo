#ifndef MEMORIA_H
#define MEMORIA_H 

#include <stdint.h>
#include <stdbool.h>
#include "instrucao.h"

#define TAM_MEMORIA 50
#define FIM_DE_INSTRUCAO -1

extern int64_t memoria[TAM_MEMORIA];

bool fimDeInstrucao(int posicao);
void memoriaInsereInst(Instrucao inst, int posicao);
void memoriaInsereDado(int valor, int posicao);
Instrucao memoriaObterInst(int posicao);
int memoriaObterDado(int posicao);
void printMemoria();

#endif
