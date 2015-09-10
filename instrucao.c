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
    char *str = malloc(sizeof(char)*MAX_STR_PRINT);
    switch(inst.opcode){
        case ADD:
            sprintf(str, "%d add R%d, R%d, R%d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case SUB:
            sprintf(str, "%d sub R%d, R%d, R%d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case MULT:
            sprintf(str, "%d mult R%d, R%d, R%d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case DIV:
            sprintf(str, "%d div R%d, R%d, R%d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case ADDI:
            sprintf(str, "%d addi R%d, R%d, %d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case SUBI:
            sprintf(str, "%d subi R%d, R%d, %d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case MULTI:
            sprintf(str, "%d multi R%d, R%d, %d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case DIVI:
            sprintf(str, "%d divi R%d, R%d, %d", inst.id, inst.dest, inst.op1, inst.op2);
            break;
        case LD:
            sprintf(str, "%d ld R%d, %d", inst.id, inst.dest, inst.op1);
            break;
        case LI:
            sprintf(str, "%d li R%d, %d", inst.id, inst.dest, inst.op1);
            break;
        case SD:
            sprintf(str, "%d sd %d, R%d", inst.id, inst.dest, inst.op1);
            break;
        case BEQ:
            sprintf(str, "%d beq R%d, R%d, %d", inst.id, inst.op1, inst.op2, inst.dest);
            break;
        case BNE:
            sprintf(str, "%d bne R%d, R%d, %d", inst.id, inst.op1, inst.op2, inst.dest);
            break;
        case BG:
            sprintf(str, "%d bg R%d, R%d, %d", inst.id, inst.op1, inst.op2, inst.dest);
            break;
        case BGE:
            sprintf(str, "%d bge R%d, R%d, %d", inst.id, inst.op1, inst.op2, inst.dest);
            break;
        case BL:
            sprintf(str, "%d bl R%d, R%d, %d", inst.id, inst.op1, inst.op2, inst.dest);
            break;
        case BLE:
            sprintf(str, "%d ble R%d, R%d, %d", inst.id, inst.op1, inst.op2, inst.dest);
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
