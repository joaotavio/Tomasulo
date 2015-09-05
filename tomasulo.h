#ifndef TOMASULO_H
#define TOMASULO_H

#include "componente.h"
#include "memoria.h"

#define NUM_COMPONENTES 9
#define MAX_STR_PRINT 50

/*** VARIÁVEIS GLOBAIS ***/

extern int qtd_busca_inst;
extern int qtd_emissao;

extern int intervalo_mem_x;
extern int intervalo_mem_y;

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
extern int ciclo_bl;
extern int ciclo_ble;
extern int ciclo_bg;
extern int ciclo_bge;
extern int ciclo_li;
extern int ciclo_lui;

extern int cont_ciclos;

void iniciarTomasulo();

#endif