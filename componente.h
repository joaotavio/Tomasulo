#ifndef COMPONENTE_H
#define COMPONENTE_H

#include <stdbool.h>
#include "instrucao.h"

#define MAX_RESERVA 100
#define	NUM_REGISTRADOR 32

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

typedef struct comp_est_reserva {
	EstacaoReserva *est_reserva;
	int tam;
	int tamMax;
} ComponenteER;

typedef struct janela {
    Instrucao *inst;
    int tam;
    int tamMax;
} Janela;

typedef struct unidade_funcional {
	Operacoes opcode;
	int vj;
	int vk;
	bool busy;
	int ciclo;
} UnidadeFuncional;

typedef struct comp_unidade_funcional {
	UnidadeFuncional *un_funcional;
	int tam;
	int tamMax;
} ComponenteUF;

typedef struct buffer{
	int origem;
	int destino;
} Buffer;

typedef struct comp_buffer {
	Buffer *buffer;
	int tam;
	int tamMax;
} ComponenteBuffer;

typedef struct un_endereco{
	Operacoes opcode;
	int origem;
	int destino;
} UnidadeEndereco;

extern Janela janela;
extern Registrador registrador[NUM_REGISTRADOR];
extern UnidadeEndereco unidadeEndereco;
extern ComponenteER est_somador;
extern ComponenteER est_multiplicador;
extern ComponenteUF somador;
extern ComponenteUF multiplicador;
extern ComponenteBuffer load;
extern ComponenteBuffer store;

/*JANELA*/
void inicializaJanela();
void janelaInsere(Instrucao inst);
Instrucao janelaRemove(int posicao);
bool janelaVazia(Janela janela);
bool janelaCheia(Janela janela);
void mostraJanela();

/*ESTAÇÃO DE RESERVA*/
void inicializaER(ComponenteER *er, ComponenteUF *uf);
void inicializaBuffer(ComponenteBuffer *buffer);
bool estReservaCheia(ComponenteER er);
bool unFuncionalCheia(ComponenteUF uf);
bool bufferCheio(ComponenteBuffer buf);
void mostraEstacao(EstacaoReserva *er, int tamanho);
void estacaoInsere(EstacaoReserva *er, Instrucao inst);

#endif
