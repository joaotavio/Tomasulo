#ifndef TOMASULO_H
#define TOMASULO_H

#include "componente.h"

//mudar defines de lugar pra arquivo se nao for usar aqui
#define NUM_COMPONENTES 7 //MUDAR

#define TAM_MEMORIA 100

/*** VARIÁVEIS GLOBAIS ***/

extern int qtd_somador;
extern int qtd_multiplicador;
extern int qtd_divisor;
extern int qtd_busca_inst;
extern int qtd_janela_inst;
extern int qtd_buffer_carga;
extern int qtd_buffer_escrita;

extern int ciclo_add;
extern int ciclo_addi;
extern int ciclo_sub;
extern int ciclo_subi;
extern int ciclo_mult;
extern int ciclo_multi;
extern int ciclo_div;
extern int ciclo_divi;
extern int ciclo_beq;
extern int ciclo_bne;
extern int ciclo_ld;
extern int ciclo_sd;

extern int cont_ciclos;

extern Componente fila;
extern Componente janela;
extern Componente load;
extern Componente store;
extern Componente somador;
extern Componente multiplicador;
extern Componente divisor;

extern char memoria[TAM_MEMORIA][100]; //fazer estrutura melhor para memoria

void insereMemoria(char memoria[][100], char valor[]);

#endif