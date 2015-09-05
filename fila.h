#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "instrucao.h"

typedef struct celula Celula;
struct celula {
    Instrucao inst;
    Celula *prox;
};

typedef struct fila *Fila;
struct fila {
    Celula *inicio;
    Celula *fim;
    int tamMax;
    int tam;
};

Fila criaFila(int tamMax);
void filaInsere(Fila f, Instrucao inst);
bool filaRemove(Fila f, Instrucao *retorno);
Instrucao filaPrimeiro(Fila f);
bool filaEstaVazia(Fila f);
bool filaEstaCheia(Fila f);
void mostraFila(Fila f);
void esvazia(Fila f);
void freeFila(Fila f);

#endif