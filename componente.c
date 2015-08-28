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

void inicializaBuffer(ComponenteBuffer *buf){
    buf->buffer = (Buffer*)calloc(buf->tamMax, sizeof(Buffer));
    buf->tam = 0;
}

bool estReservaCheia(ComponenteER er){
    return er.tam == er.tamMax;
}

bool unFuncionalCheia(ComponenteUF uf){
    return uf.tam == uf.tamMax;
}

bool bufferCheio(ComponenteBuffer buf){
    return buf.tam == buf.tamMax;
}

void estacaoInsere(EstacaoReserva *er, Instrucao inst){
    //er[0].opcode = inst.opcode;
    //er = 1;
    printf("oi!!");
    //er[0].vj = inst.op1;
    //er[0].vk = inst.op2;
    //er[0].busy = true;
}

void mostraEstacao(EstacaoReserva *er, int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        printf("Vk: %d\n", er[0].vk);
    }
}

//fazer função procura estacao livre
//procura unidade livre
//estacao cheia, vazia