#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

Fila criaFila(int tamMax){
    Fila fila;
    fila = malloc(sizeof(Fila));
    fila->tamMax = tamMax;
    fila->tam = 0;
    fila->inicio = malloc(sizeof(Celula));
    fila->fim = fila->inicio;
    fila->inicio->dado = NULL;
    fila->inicio->prox = NULL;
    return fila;
}

void filaInsere(Fila f, char *valor){
    if (filaEstaCheia(f))
        return;
    
    Celula *nova;
    nova = malloc(sizeof(Celula));
    nova->dado = valor;
    nova->prox = NULL;

    if (filaEstaVazia(f)){
        Celula *aux = f->inicio;
        f->inicio = nova;
        free(aux);
    }
    else
        f->fim->prox = nova;

    f->fim = nova;
    printf("INICIO: %d\n", f->inicio);
    printf("INICIO PROX: %d\n", f->inicio->prox);
    printf("FIM: %d\n", f->fim);
    printf("FIM PROX: %d\n\n", f->fim->prox);
    f->tam++;
}

char* filaRemove(Fila f){
    if (filaEstaVazia(f))
        return NULL;

    Celula *alvo = f->inicio;
    char *s = alvo->dado;

    if (f->inicio == f->fim){
        f->inicio = malloc(sizeof(Celula));
        f->inicio->dado = NULL;
        f->inicio->prox = NULL;
    } else
        f->inicio = f->inicio->prox;

    free(alvo);
    f->tam--;
    return s;
}

int filaEstaVazia(Fila f){
    return f->tam == 0;
}

int filaEstaCheia(Fila f){
    return f->tam == f->tamMax;
}

void mostraFila(Fila f){
    Celula *aux = f->inicio;
    while (aux != NULL){
        printf("%s\n", aux->dado);
        aux = aux->prox;
    }
}

void esvazia(Fila f){
    while (f->tam > 0)
        filaRemove(f);
}


//int main()
//{
//    Fila f = criaFila(5);
//    mostraFila(f);
//    filaInsere(f, "HELLO");
//    filaInsere(f, "HELLO");
//    filaInsere(f, "HELLO");
//    filaInsere(f, "kkk");
//    mostraFila(f);
//    esvazia(f);
//    mostraFila(f);
//    /*printf("REMOVEU: %s\n\n", filaRemove(f));
//    mostraFila(f);
//    printf("REMOVEU: %s\n\n", filaRemove(f));
//    mostraFila(f);
//    printf("REMOVEU: %s\n\n", filaRemove(f));
//    mostraFila(f);
//    printf("REMOVEU: %s\n\n", filaRemove(f));
//    mostraFila(f);
//    printf("REMOVEU: %s\n\n", filaRemove(f));
//    mostraFila(f);
//    printf("REMOVEU: %s\n\n", filaRemove(f));
//    mostraFila(f);*/
//
//    /*char *t = "HELLO";
//    printf("%s\n", t);
//    for (int i = 0; t[i] != '\0'; ++i)
//    {
//        printf("%c\n", t[i]);
//    }*/
//
//    getchar();
//    return 0;
//}