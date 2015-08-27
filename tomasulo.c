#include <stdio.h>
#include <stdlib.h>
#include "tomasulo.h"

/*** VARIÁVEIS GLOBAIS ***/

int qtd_somador;
int qtd_multiplicador;
int qtd_divisor;
int qtd_busca_inst;
int qtd_buffer_carga;
int qtd_buffer_escrita;
int qtd_emissao;
int qtd_portas_reg;

int intervalo_mem_x;
int intervalo_mem_y;

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

/*Componente janela;
Componente load;
Componente store;
Componente somador;
Componente multiplicador;*/

int cont_ciclos;
int pc;

bool flag_exit;

//busca instruções na memória e coloca na fila
void busca(){
    Instrucao inst;
    int i;
    for (i = 0; i < qtd_busca_inst; i++){
        if (!janelaCheia(janela)){
            inst = memoriaObterInst(pc);
            switch (inst.opcode){
                case EXIT:
                    flag_exit = true;
                    return;
                    break;
                case JUMP:
                    //JUMP CONTA COMO UMA INSTRUCAO BUSCADA?
                    pc += inst.dest;
                    break;
                case NOP:
                    pc++;
                    break;
                default:
                    janelaInsere(inst);
                    pc++;
                    break;
            }
        }
    }
}

void emissao(){
    
}

void iniciarTomasulo(){
    pc = 0;
    flag_exit = false;
    Instrucao inst;
    //loop principal
    while (true) {
        busca();
        mostraJanela();
        if (flag_exit)
            break;
        printf("-----------------------\n");
        //filaRemove(fila, &inst);
		inst = janelaRemove(0);
        printf("\nREMOVIDA: ");
        printInstrucao(inst);
    }
}