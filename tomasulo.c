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

int cont_ciclos;

Componente fila;
Componente janela;
Componente load;
Componente store;
Componente somador;
Componente multiplicador;
Componente divisor;
