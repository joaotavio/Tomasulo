#include <stdio.h>
#include "instrucao.h"
#include "util.h"
#include "tomasulo.h"

inst_t instrucaoParaBinario(Instrucao inst){
    inst_t binario;
    int nbit_registrador = calculaBits(NUM_REGISTRADOR-1);

    binario = ((inst_t)inst.opcode << (nbit_registrador + nbit_registrador + BITS_IMEDIATO));

    switch(inst.opcode){
        case ADD:
        case SUB:
        case MULT:
        case DIV:
            binario |= ((inst_t)inst.dest << (nbit_registrador + BITS_IMEDIATO));
            binario |= ((inst_t)inst.op1 << BITS_IMEDIATO);
            binario |= ((inst_t)inst.op2 << (BITS_IMEDIATO - nbit_registrador));
            break;
        case ADDI:
        case SUBI:
        case MULTI:
        case DIVI:
            binario |= ((inst_t)inst.dest << (nbit_registrador + BITS_IMEDIATO));
            binario |= ((inst_t)inst.op1 << BITS_IMEDIATO);
            binario |= LAST((inst_t)inst.op2, BITS_IMEDIATO);
            break;
        case LD:
        case LI:
            binario |= ((inst_t)inst.dest << (BITS_IMEDIATO + nbit_registrador));
            binario |= LAST((inst_t)inst.op1, BITS_IMEDIATO);
            break;
        case SD:
            binario |= (inst_t)inst.op1 << (BITS_IMEDIATO + nbit_registrador);
            binario |= (inst_t)inst.dest;
            break;
        case BEQ:
        case BNE:
        case BG:
        case BGE:
        case BL:
        case BLE:
            binario |= ((inst_t)inst.op1 << (nbit_registrador + BITS_IMEDIATO));
            binario |= ((inst_t)inst.op2 << BITS_IMEDIATO);
            binario |= LAST((inst_t)inst.dest, BITS_IMEDIATO);
            break;
        case JUMP:
            binario |=  LAST((inst_t)inst.dest, BITS_IMEDIATO);
            break;
        case EXIT:
        case NOP: break;
    }

    return binario;
}

Instrucao binarioParaInstrucao(inst_t valor){
    Instrucao inst;
    int nbit_operacoes = calculaBits(NUM_OPERACOES-1);
    int nbit_registrador = calculaBits(NUM_REGISTRADOR-1);

    inst.opcode = LAST(valor >> (nbit_registrador + nbit_registrador + BITS_IMEDIATO), nbit_operacoes);

    switch(inst.opcode){
        case ADD:
        case SUB:
        case MULT:
        case DIV:
            inst.dest = LAST(valor >> (nbit_registrador + BITS_IMEDIATO), nbit_registrador);
            inst.op1 = LAST(valor >> BITS_IMEDIATO, nbit_registrador);
            inst.op2 = LAST(valor >> (BITS_IMEDIATO - nbit_registrador), nbit_registrador);
            break;
        case ADDI:
        case SUBI:
        case MULTI:
        case DIVI:
            inst.dest = LAST(valor >> (nbit_registrador + BITS_IMEDIATO), nbit_registrador);
            inst.op1 = LAST(valor >> BITS_IMEDIATO, nbit_registrador);
            inst.op2 = (imediato_t)LAST(valor, BITS_IMEDIATO);
            break;
        case LD:
        case LI:
            inst.dest = LAST(valor >> (BITS_IMEDIATO + nbit_registrador), nbit_registrador);
            inst.op2 = LAST(valor >> BITS_IMEDIATO, nbit_registrador);
            inst.op1 = (imediato_t)LAST(valor, BITS_IMEDIATO);
            break;
        case SD:
            inst.op1 = LAST(valor >> (BITS_IMEDIATO + nbit_registrador), nbit_registrador);
            inst.op2 = LAST(valor >> BITS_IMEDIATO, nbit_registrador);
            inst.dest = LAST(valor, BITS_IMEDIATO);
            break;
        case BEQ:
        case BNE:
        case BG:
        case BGE:
        case BL:
        case BLE:
            inst.op1 = LAST(valor >> (nbit_registrador + BITS_IMEDIATO), nbit_registrador);
            inst.op2 = LAST(valor >> BITS_IMEDIATO, nbit_registrador);
            inst.dest = (imediato_t)LAST(valor, BITS_IMEDIATO);
            break;
        case JUMP:
            inst.op1 = 0;
            inst.op2 = 0;
            inst.dest = (imediato_t)LAST(valor, BITS_IMEDIATO);
            break;
        case EXIT:
            inst.op1 = 0;
            inst.op2 = 0;
            inst.dest = 0;
            break;
        case NOP: 
            inst.op1 = 0;
            inst.op2 = 0;
            inst.dest = 0;
            break;
    }

    return inst;
}

char* instToString(Instrucao inst){
    char *str = (char*)malloc(sizeof(char));
    switch(inst.opcode){
        case ADD:
            sprintf(str, "add R%d, R%d, R%d", inst.dest, inst.op1, inst.op2);
            break;
        case SUB:
            sprintf(str, "sub R%d, R%d, R%d", inst.dest, inst.op1, inst.op2);
            break;
        case MULT:
            sprintf(str, "mult R%d, R%d, R%d", inst.dest, inst.op1, inst.op2);
            break;
        case DIV:
            sprintf(str, "div R%d, R%d, R%d", inst.dest, inst.op1, inst.op2);
            break;
        case ADDI:
            sprintf(str, "addi R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case SUBI:
            sprintf(str, "subi R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case MULTI:
            sprintf(str, "multi R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case DIVI:
            sprintf(str, "divi R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case LD:
            sprintf(str, "ld R%d, %d", inst.dest, inst.op1);
            break;
        case LI:
            sprintf(str, "li R%d, %d", inst.dest, inst.op1);
            break;
        case SD:
            sprintf(str, "sd %d, R%d", inst.dest, inst.op1);
            break;
        case BEQ:
            sprintf(str, "beq R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case BNE:
            sprintf(str, "bne R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case BG:
            sprintf(str, "bg R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case BGE:
            sprintf(str, "bge R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case BL:
            sprintf(str, "bl R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case BLE:
            sprintf(str, "ble R%d, R%d, %d", inst.dest, inst.op1, inst.op2);
            break;
        case JUMP:
            sprintf(str, "jump %d", inst.dest);
            break;
        case EXIT:
            sprintf(str, "exit");
            break;
        case NOP: 
            sprintf(str, "nop");
            break;
    }
    return str;
}

void printInstrucao(Instrucao inst){
    char *str = instToString(inst);
    printf("%s\n", str);
    free(str);
}