#ifndef INSTRUCAO_H
#define INSTRUCAO_H 

#define NUM_OPERACOES 12

typedef enum operacoes {OP_INVALIDO, ld, sd, beq, bne, add, addi, sub, subi, mult, multi, Div, divi} Operacoes;

typedef struct instrucao {
    //guardar id da instrucao para quando tiver dependencia verdadeira
    int id;
    Operacoes opcode;
    char *op1; //MUDAR ESSES NOMES e mudar o tipo
    char *op2;
    char *op3;
} Instrucao;

#endif