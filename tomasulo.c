#include <stdio.h>
#include <stdlib.h>
#include "tomasulo.h"

/*** VARIÁVEIS GLOBAIS ***/

int qtd_somador;
int qtd_multiplicador;
int qtd_divisor;
int qtd_busca_inst;
int qtd_janela_inst;
int qtd_buffer_carga;
int qtd_buffer_escrita;
int qtd_emissao;

int tam_memoria;

int ciclo_add;
int ciclo_addi;
int ciclo_sub;
int ciclo_subi;
int ciclo_mult;
int ciclo_multi;
int ciclo_div;
int ciclo_divi;
int ciclo_beq;
int ciclo_bne;
int ciclo_ld;
int ciclo_sd;
int ciclo_bl;
int ciclo_ble;
int ciclo_bg;
int ciclo_bge;
int ciclo_li;
int ciclo_lui;

int cont_ciclos;

Componente fila;
Componente janela;
Componente load;
Componente store;
Componente somador;
Componente multiplicador;
Componente divisor;

int pc;

//busca instruções na memória e coloca na fila
void busca(){
    Instrucao inst;
    int i;
    for (i = 0; i < qtd_busca_inst; i++){
        if (fimDeInstrucao(pc))
            return;
        
        inst = memoriaObterInst(pc);
        //POR ALGUM MOTIVO AS VEZES TA TRAVANDO E AS VEZES FUNCIONA
        componenteInsereFim(fila, inst);
        pc++;
    }
}

void iniciarTomasulo(){
    pc = 0;
    //loop principal
    while (true) {
        busca();
        //APENAS PARA TESTAR
        if (fimDeInstrucao(pc))
            break;
    }
    mostraComponente(fila);
}