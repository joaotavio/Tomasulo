#ifndef MEMORIA_H
#define MEMORIA_H 

#include <stdbool.h>
#include <stdint.h>
#include "instrucao.h"

#define TAM_MEMORIA 50

extern int64_t memoria[TAM_MEMORIA];

bool memoriaInsereInst(Instrucao inst, int posicao);
bool memoriaInsereDado(int valor, int posicao);
bool memoriaObterInst(int posicao, Instrucao *inst_retorno);
bool memoriaObterDado(int posicao, int *valor_retorno);
void printMemoria();

#endif

/*
codificacao:
ld = 0000
sd = 0001
beq = 0010
bne = 0011
add = 0100
addi = 0101
sub = 0110
subi = 0111
mult = 1000
multi = 1001
Div = 1010
divi = 1011

*/