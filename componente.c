#include <stdio.h>
#include <stdlib.h>
#include "componente.h"

bool componenteEstaVazio(Componente comp){
    return comp->tam == 0;
}

bool componenteEstaCheio(Componente comp){
    return comp->tam == comp->tamMax;
}

Componente criaComponente(int tamMax){
    Componente componente;
    componente = malloc(sizeof(Componente));
    componente->tamMax = tamMax;
    componente->tam = 0;
    return componente;
}

void componenteInsereFim(Componente comp, Instrucao inst){
    if(componenteEstaCheio(comp))
        return;

    Campo *novo;
    novo = malloc(sizeof(Campo));
    novo->dado = inst;
    novo->prox = NULL;

    if (componenteEstaVazio(comp))
        comp->inicio = novo;
    else 
        comp->fim->prox = novo;
    
    comp->fim = novo;

    comp->tam++;
}

void componenteInsereInicio(Componente comp, Instrucao inst){
    if(componenteEstaCheio(comp))
        return;

    Campo *novo;
    novo = malloc(sizeof(Campo));
    novo->dado = inst;
    novo->prox = NULL;

    if (componenteEstaVazio(comp))
        comp->fim = novo;
    else 
        novo->prox = comp->inicio;
    
    comp->inicio = novo;

    comp->tam++;
}

Instrucao componenteRemoveInicio(Componente comp){
    Instrucao retorno = {0};

    if (componenteEstaVazio(comp))
        return retorno;

    Campo *alvo = comp->inicio;
    retorno = alvo->dado;

    if (comp->inicio == comp->fim)
        comp->fim = NULL;

    comp->inicio = comp->inicio->prox;

    free(alvo);
    
    comp->tam--;
    return retorno;
}

Instrucao componenteRemoveEspecifico(Componente comp, int id){
    if (comp->inicio->dado.id == id)
        return componenteRemoveInicio(comp);
    
    Instrucao retorno = {0};

    Campo *alvo = comp->inicio->prox;
    Campo *anterior = comp->inicio;

    while(alvo != NULL && alvo->dado.id != id){
        anterior = alvo;
        alvo = alvo->prox;
    }

    if (alvo != NULL){
        anterior->prox = alvo->prox;
        if (alvo == comp->fim)
            comp->fim = anterior;

        retorno = alvo->dado;
        free(alvo);
        comp->tam--;
        return retorno;
    }

    return retorno;
}


//MELHORAR O PRINT DESSA FUNCAO
void mostraComponente(Componente comp){
    if (componenteEstaVazio(comp)){
        printf("Componente vazio.\n");
        return;
    }

    Campo *aux = comp->inicio;
    printf("\nComponentes: \n");
    printf("Tamanho: %d\n", comp->tam);
    while(aux != NULL){
        printf("ID: %d\n", aux->dado.id);
        printf("OPCODE: %d\n", aux->dado.opcode);
        printf("DEST: %lld\n", aux->dado.dest);
        printf("OP1: %lld\n", aux->dado.op1);
        printf("OP2: %lld\n\n", aux->dado.op2);
        aux = aux->prox;
    }
}

void esvaziaComponente(Componente comp){
    while(comp->tam > 0)
        componenteRemoveInicio(comp);
}

void freeComponente(Componente comp){
    esvaziaComponente(comp);
    free(comp);
}