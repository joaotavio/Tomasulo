#include <stdio.h>
#include <stdlib.h>
#include "componente.h"

int tam_janela;

Janela janela;
EstacaoReserva *est_somador;
EstacaoReserva *est_multiplicador;
Registrador registrador[TAM_REGISTRADOR];
UnidadeFuncional *somador;
UnidadeFuncional *multiplicador;
UnidadeEndereco unidadeEndereco;
Buffer *load;
Buffer *store;

void inicializaJanela(int tamanho){
    janela.inst = (Instrucao*)calloc(tamanho, sizeof(Instrucao));
    janela.tam = 0;
}

void janelaInsere(Instrucao inst){
    janela.inst[janela.tam] = inst;
    janela.tam++;
}

Instrucao janelaRemove(int posicao){
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
    return janela.tam == tam_janela;
}

void mostraJanela(){
    int i;
    for (i = 0; i < janela.tam; ++i) {
        printInstrucao(janela.inst[i]);
    }
}

void inicializaER(EstacaoReserva *er, UnidadeFuncional *uf, int tamanho){
    er = (EstacaoReserva*)calloc(tamanho, sizeof(EstacaoReserva));
    uf = (UnidadeFuncional*)calloc(tamanho, sizeof(UnidadeFuncional));
}

void inicializaBuffer(Buffer *buffer, int tamanho){
    buffer = (Buffer*)calloc(tamanho, sizeof(Buffer));
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
