#ifndef TOMASULO_H
#define TOMASULO_H

#include "fila.h"
#include "componente.h"
#include "memoria.h"

#define NUM_COMPONENTES 11
#define NUM_REGISTRADOR 32

/*** VARIÁVEIS GLOBAIS ***/

extern int qtd_somador;
extern int qtd_multiplicador;
extern int qtd_busca_inst;
extern int tam_janela_inst;
extern int qtd_buffer_carga;
extern int qtd_buffer_escrita;
extern int qtd_emissao;
extern int tam_fila;
extern int qtd_portas_reg;

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

extern Fila fila;
extern Componente janela;
extern Componente load;
extern Componente store;
extern Componente somador;
extern Componente multiplicador;

void iniciarTomasulo();

#endif