#ifndef MEMORIA_H
#define MEMORIA_H 

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "instrucao.h"

typedef struct mem_exec {
    int id;
    Operacoes opcode;
    int origem;
    int destino;
    bool busy;
    int ciclos;
} MemoriaExec;

extern MemoriaExec mem_exec;
extern int tam_memoria;
extern uint8_t* memoria;

void inicializaMemoria(int tam_memoria);
void memoriaInsereInst(Instrucao inst, int posicao);
void memoriaInsereDado(int valor, int posicao);
Instrucao memoriaObterInst(int posicao);
int memoriaObterDado(int posicao);

void memoriaExecInsere(MemoriaExec *mem_exec, int id, Operacoes opcode, int origem, int destino, int ciclos);
void memoriaExecRemove(MemoriaExec *mem_exec);
char* memToString(MemoriaExec mem_exec);

#endif
