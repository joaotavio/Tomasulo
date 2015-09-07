#ifndef INSTRUCAO_H
#define INSTRUCAO_H 

#include <stdint.h>

#define NUM_OPERACOES 20
#define BITS_IMEDIATO sizeof(imediato_t)*8
#define BITS_INSTRUCAO sizeof(inst_t)*8

#define LAST(k, n) ((k) & (((inst_t)1<<(n))-1))

typedef int16_t imediato_t;
typedef int32_t inst_t;
typedef enum operacoes {LD, SD, LI, BEQ, BNE, BG, BGE, BL, BLE, JUMP, EXIT, ADD, ADDI, SUB, SUBI, MULT, MULTI, DIV, DIVI, NOP} Operacoes;

typedef struct instrucao {
    //guardar id da instrucao para quando tiver dependencia verdadeira
    int id;
    Operacoes opcode;
    int dest;
    int op1;
    int op2;
} Instrucao;

inst_t instrucaoParaBinario(Instrucao inst);
Instrucao binarioParaInstrucao(inst_t valor);
char* instToString(Instrucao inst);
void printInstrucao(Instrucao inst);

#endif

/* fazer funcao transformar binario aqui (receber string e retornar binario)



*/