#ifndef COMPONENTE_H
#define COMPONENTE_H

#include <stdbool.h>
#include "instrucao.h"

#define MAX_RESERVA 100
#define	NUM_REGISTRADOR 32

typedef struct registrador {
	int64_t valor;
	int qk[MAX_RESERVA];
	int tamFila;
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
	int ciclos;
} UnidadeFuncional;

typedef struct comp_unidade_funcional {
	UnidadeFuncional *un_funcional;
	int tam;
	int tamMax;
} ComponenteUF;

typedef struct buffer{
	int origem;
	int destino;
	bool busy;
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
extern ComponenteER er_somador;
extern ComponenteER er_multiplicador;
extern ComponenteUF somador;
extern ComponenteUF multiplicador;
extern ComponenteBuffer load;
extern ComponenteBuffer store;

/* JANELA */
void inicializaJanela();
void janelaInsere(Instrucao inst);
Instrucao janelaRemove(int posicao);
bool janelaVazia(Janela janela);
bool janelaCheia(Janela janela);
void mostraJanela();

/* ESTAÇÃO DE RESERVA */
void inicializaER(ComponenteER *er, ComponenteUF *uf);
void estacaoInsere(ComponenteER *er, Operacoes opcode, int qj, int qk, int vj, int vk, int posicao);
void estacaoRemove(ComponenteER *er, int posicao);
int procuraEstacao(ComponenteER er);
bool estReservaCheia(ComponenteER er);
void mostraEstacao(ComponenteER *er, int tamanho);
char* erToString(EstacaoReserva er);

/* UNIDADE FUNCIONAL */
bool unFuncionalCheia(ComponenteUF uf);
int procuraUF(ComponenteUF uf, int posicao);
void uFuncionalInsere(ComponenteUF *uf, int posicao, Operacoes opcode, int vj, int vk, int ciclos);
void uFuncionalRemove(ComponenteUF *uf, int posicao);
char* ufToString(UnidadeFuncional uf);

/* BUFFER LOAD/STORE */
void inicializaBuffer(ComponenteBuffer *buffer);
bool bufferCheio(ComponenteBuffer buf);
void bufferInsere(ComponenteBuffer *buf, int origem, int destino);
int procuraBuffer(ComponenteBuffer buf);

/* REGISTRADOR */
void insereFilaRegistrador(Registrador *reg, int posicao, int estacao);

#endif
