#include <stdio.h>
#include <stdlib.h>
#include "tomasulo.h"

/*** VARIÁVEIS GLOBAIS ***/

int qtd_busca_inst;
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

int cont_ciclos;
int pc;

/*VARIÁVEIS PARA PRINT*/

Instrucao emitidas[10]; //DEPOIS MUDAR ISSO PARA MALLOC - TALVEZ PODE USAR ISSO PRA DEPENDENCIA
int num_emitidas;

void printCiclo(){
    int i;
    char *strInst;
    printf("CICLO: %d\n", cont_ciclos);
    printf("%-20s | %-20s | %-20s | %-20s | %-20s |\n", "EMISSAO", "UNIDADE DE ENDERECO", "ESTACAO DE RESERVA", "UNIDADE FUNCIONAL", "ESCRITA");
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|\n");
    //ACHAR MINIMO ENTRE, EMITIDAS, UE, TAM ER, TAM UF E ESCRITAS
    for (i = 0; i < num_emitidas; ++i) {
        strInst = instToString(emitidas[i]);
        printf("%-20s | ", strInst);
        printf("%-20s | ", "---"); //UE
        printf("%-20s | ", "---"); //ER
        printf("%-20s | ", "---"); //UF
        printf("%-20s | ", "---"); //ESCR
        printf("\n");
        free(strInst);
    }
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|\n\n");
}

void printRegistrador(){

}

//busca instruções na memória e coloca na janela
bool busca(){
    Instrucao inst;
    int i;
    for (i = 0; i < qtd_busca_inst && !janelaCheia(janela); i++){
        inst = memoriaObterInst(pc);
        switch (inst.opcode){
            case EXIT:
                return !janelaVazia(janela);
                break;
            case JUMP:
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
    return !janelaVazia(janela);
}

bool emissao(){
    int i;
    Instrucao inst;

    num_emitidas = 0;
    for(i = 0; i < qtd_emissao && !janelaVazia(janela); i++, num_emitidas++){
        inst = janelaRemove(0);
        emitidas[i] = inst;
        switch (inst.opcode){
            case LD:
                //bufferInsere(load, inst);
                break;
            case SD:
                //bufferInsere(store, inst);
                break;
            case LI:
                //estacaoInsere(est_somador, inst);
                break;
            case BEQ:
                //estacaoInsere(est_somador, inst);
                break;
            case BNE:
                //estacaoInsere(est_somador, inst);
                break;
            case BG:
                //estacaoInsere(est_somador, inst);
                break;
            case BGE:
                //estacaoInsere(est_somador, inst);
                break;
            case BL:
                //estacaoInsere(est_somador, inst);
                break;
            case BLE:
                //estacaoInsere(est_somador, inst);
                break;
            case ADD:
                //estacaoInsere(est_somador, inst);
                break;
            case ADDI:
                //estacaoInsere(est_somador, inst);
                break;
            case SUB:
                //estacaoInsere(est_somador, inst);
                break;
            case SUBI:
                //estacaoInsere(est_somador, inst);
                break;
            case MULT:
                //estacaoInsere(est_multiplicador, inst);
                break;
            case MULTI:
                //estacaoInsere(est_multiplicador, inst);
                break;
            case DIV:
                //estacaoInsere(est_multiplicador, inst);
                break;
            case DIVI:
                //estacaoInsere(est_multiplicador, inst);
                break;
            default:
                break;
        }
    }
    return num_emitidas != 0;
}

void pulso(){

}

void iniciarTomasulo(){
    pc = 0;
    cont_ciclos = 0;
    bool flag_busca, flag_emissao, flag_exec = false, flag_escr = false;
    Instrucao inst;

    //loop principal
    while (true) {
        flag_emissao = emissao();
        flag_busca = busca();
        if (!flag_busca && !flag_emissao && !flag_exec && !flag_escr)
            break;
        cont_ciclos++;
        printCiclo();
    }
}