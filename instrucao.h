#ifndef INSTRUCAO_H
#define INSTRUCAO_H 

#define NUM_OPERACOES 12

typedef enum operacoes {LD, SD, BEQ, BNE, ADD, ADDI, SUB, SUBI, MULT, MULTI, DIV, DIVI} Operacoes;

typedef struct instrucao {
    //guardar id da instrucao para quando tiver dependencia verdadeira
    int id;
    Operacoes opcode;
    int dest;
    int op1;
    int op2;
} Instrucao;

#endif