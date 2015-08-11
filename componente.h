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

struct registrador{
	int valor;
	reserva[50] relacao; //estação de reservas que irá escrever no registrador
}

struct reserva{
	Operacoes opcode;
	Registrador registrador_j;
	Registrador registrador_k;
	int valor_j;
	int valor_k;
	bool busy;
}

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
