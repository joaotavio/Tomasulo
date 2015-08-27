#ifndef COMPONENTE_H
#define COMPONENTE_H

#include <stdbool.h>
#include "instrucao.h"

#define MAX_RESERVA 100
#define	TAM_REGISTRADOR 32

typedef struct registrador {
	int valor;
	int qk[MAX_RESERVA];
} Registrador;

typedef struct est_reserva{
	Operacoes opcode;
	int qj;
	int qk;
	int vj;
	int vk;
	bool busy;
    int A;
} EstacaoReserva;

typedef struct janela {
    Instrucao *inst;
    int tam;
} Janela;

typedef struct unidadeFuncional {
	Operacoes opcode;
	int vj;
	int vk;
	bool busy;
	int ciclo;
} UnidadeFuncional;

typedef struct buffer{
	int origem;
	int destino;
} Buffer;

typedef struct unEndereco{
	Operacoes opcode;
	int origem;
	int destino;
} UnidadeEndereco;

extern int tam_janela;

extern Janela janela;
extern EstacaoReserva *est_somador;
extern EstacaoReserva *est_multiplicador;
extern Registrador registrador[TAM_REGISTRADOR];
extern UnidadeFuncional *somador;
extern UnidadeFuncional *multiplicador;
extern UnidadeEndereco unidadeEndereco;
extern Buffer *load;
extern Buffer *store;

/*JANELA*/
void inicializaJanela(int tamanho);
void janelaInsere(Instrucao inst);
Instrucao janelaRemove(int posicao);
bool janelaVazia(Janela janela);
bool janelaCheia(Janela janela);
void mostraJanela();

/*ESTAÇÃO DE RESERVA*/
void inicializaER(EstacaoReserva *er, UnidadeFuncional *uf, int tamanho);
void inicializaBuffer(Buffer *buffer, int tamanho);

#endif
