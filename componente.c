#include <stdio.h>
#include <stdlib.h>
#include "componente.h"

Janela janela;
Registrador registrador[NUM_REGISTRADOR];
UnidadeEndereco unidadeEndereco;
ComponenteER est_somador;
ComponenteER est_multiplicador;
ComponenteUF somador;
ComponenteUF multiplicador;
ComponenteBuffer load;
ComponenteBuffer store;


/* JANELA */
void inicializaJanela(){
    janela.inst = (Instrucao*)calloc(janela.tamMax, sizeof(Instrucao));
    janela.tam = 0;
}

void janelaInsere(Instrucao inst){
    janela.inst[janela.tam] = inst;
    janela.tam++;
}

Instrucao janelaRemove(int posicao){
    if (janelaVazia(janela))
        return (Instrucao){0};
    Instrucao retorno = janela.inst[posicao];
	int i;
	
	for(i = posicao; i < (janela.tam - 1); i++){
		janela.inst[i] = janela.inst[i+1];
	}
	
	janela.tam--;
	
    return retorno;
}

bool janelaVazia(Janela janela){
    return janela.tam == 0;
}

bool janelaCheia(Janela janela){
    return janela.tam == janela.tamMax;
}

void mostraJanela(){
    int i;
    for (i = 0; i < janela.tam; ++i) {
        printInstrucao(janela.inst[i]);
    }
}

/* ESTAÇÃO DE RESERVA */
void inicializaER(ComponenteER *er, ComponenteUF *uf){
    er->est_reserva = (EstacaoReserva*)calloc(er->tamMax, sizeof(EstacaoReserva));
    uf->un_funcional = (UnidadeFuncional*)calloc(er->tamMax, sizeof(UnidadeFuncional));
    er->tam = 0;
    uf->tam = 0;
    uf->tamMax = er->tamMax;
}

bool estReservaCheia(ComponenteER er){
    return er.tam == er.tamMax;
}

void estacaoInsere(ComponenteER er, Instrucao inst, int posicao){
	er.est_reserva[posicao].opcode = inst.opcode;
	er.est_reserva[posicao].qj = inst.op1;
	er.est_reserva[posicao].qk = inst.op2;
	er.est_reserva[posicao].busy = true;
}

int procuraEstacao(ComponenteER er){
	int i;
	for(i = 0; i < er.tamMax; i++){
		if(er.est_reserva[i].busy == false){
			return i;
		}
	}
	
	return -1;
}

/*void mostraEstacao(ComponenteBuffer *er, int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        printf("Vk: %d\n", er[0].vk);
    }
}*/


/*Unidade Funcional*/
bool unFuncionalCheia(ComponenteUF uf){
    return uf.tam == uf.tamMax;
}

/*Buffer Load/Store*/
void inicializaBuffer(ComponenteBuffer *buf){
    buf->buffer = (Buffer*)calloc(buf->tamMax, sizeof(Buffer));
    buf->tam = 0;
}

bool bufferCheio(ComponenteBuffer buf){
    return buf.tam == buf.tamMax;
}
//fazer função procura estacao livre
//procura unidade livre
//estacao cheia, vazia