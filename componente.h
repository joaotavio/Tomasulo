#ifndef COMPONENTE_H
#define COMPONENTE_H

#include <stdbool.h>
#include "instrucao.h"

typedef struct campo Campo;
struct campo {
    Instrucao dado;
    Campo *prox;
};

typedef struct componente *Componente;
struct componente {
    Campo *inicio;
    Campo *fim;

    int tamMax;
    int tam;
};

bool componenteEstaVazio(Componente comp);
bool componenteEstaCheio(Componente comp);
Componente criaComponente(int tamMax);
void componenteInsereInicio(Componente comp, Instrucao inst);
void componenteInsereFim(Componente comp, Instrucao inst);
Instrucao componenteRemoveInicio(Componente comp);
Instrucao componenteRemoveEspecifico(Componente comp, int id);
void mostraComponente(Componente comp);
void esvaziaComponente(Componente comp);
void freeComponente(Componente comp);

#endif
