#ifndef INSTRUCAO_H
#define INSTRUCAO_H 

#include <stdint.h>

#define NUM_OPERACOES 19

typedef enum operacoes {NOP, LD, SD, BEQ, BNE, ADD, ADDI, SUB, SUBI, MULT, MULTI, DIV, DIVI, BL, BLE, BG, BGE, LI, LUI} Operacoes;

typedef struct instrucao {
    //guardar id da instrucao para quando tiver dependencia verdadeira
    int id;
    Operacoes opcode;
    long long int dest;
    long long int op1;
    long long int op2;
} Instrucao;

int64_t instrucaoParaBinario(Instrucao inst);
Instrucao binarioParaInstrucao(int64_t valor);

#endif

/* fazer funcao transformar binario aqui (receber string e retornar binario)*/

/*
codificacao:
LD    = 00000
SD    = 00001
BEQ   = 00010
BNE   = 00011
ADD   = 00100
ADDI  = 00101
SUB   = 00110
SUBI  = 00111
MULT  = 01000
MULTI = 01001
DIV   = 01010
DIVI  = 01011
BL    = 01100
BLE   = 01101
BG    = 01110
BGE   = 01111
LI    = 10000
LUI   = 10001
NOP   = 10010

*/