#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

Fila criaFila(int tamMax){
    Fila fila;
    fila = malloc(sizeof(struct fila));
    fila->tamMax = tamMax;
    fila->tam = 0;
    return fila;
}

void filaInsere(Fila f, Instrucao valor){
    if (filaEstaCheia(f))
        return;
    
    Celula *nova;
    nova = malloc(sizeof(Celula));
    nova->inst = valor;
    nova->prox = NULL;

    if (filaEstaVazia(f))
        f->inicio = nova;    
    else
        f->fim->prox = nova;

    f->fim = nova;
    f->tam++;
}

bool filaRemove(Fila f, Instrucao *retorno){
    if (filaEstaVazia(f))
        return false;

    Celula *alvo = f->inicio;
    *retorno = alvo->inst;

    if (f->inicio == f->fim)
        f->fim = NULL;

    f->inicio = f->inicio->prox;

    free(alvo);
    f->tam--;
    return true;
}

Instrucao filaPrimeiro(Fila f){
    if (filaEstaVazia(f))
        return (Instrucao){0};

    return f->inicio->inst;
}

bool filaEstaVazia(Fila f){
    return f->tam == 0;
}

bool filaEstaCheia(Fila f){
    return f->tam == f->tamMax;
}

void esvazia(Fila f){
    Instrucao inst;
    while (f->tam > 0)
        filaRemove(f, &inst);
}

void freeFila(Fila f){
    esvazia(f);
    free(f);
}
