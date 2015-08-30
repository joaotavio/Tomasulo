#ifndef FILA_H
#define FILA_H

#include <stdbool.h>

typedef struct celula Celula;
struct celula {
    int dado;
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
void filaInsere(Fila f, int valor);
bool filaRemove(Fila f, int *retorno);
bool filaEstaVazia(Fila f);
bool filaEstaCheia(Fila f);
void mostraFila(Fila f);
void esvazia(Fila f);
void freeFila(Fila f);

#endif