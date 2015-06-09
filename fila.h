#ifndef FILA_H
#define FILA_H

#define MAX_CHAR 100

typedef struct celula Celula;
struct celula {
    char *dado;
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
void filaInsere(Fila f, char *valor);
char* filaRemove(Fila f);
int filaEstaVazia(Fila f);
int filaEstaCheia(Fila f);
void mostraFila(Fila f);
void esvazia(Fila f);

#endif