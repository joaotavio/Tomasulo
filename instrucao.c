#include "instrucao.h"
#include "util.h"
#include "tomasulo.h"

#define BITS_INTEIRO 32
#define LAST(k,n) ((k) & (((int64_t)1<<(n))-1))
//fazer define COMBINAR

int64_t instrucaoParaBinario(Instrucao inst){
    int64_t binario;
    int nbit_registrador = calculaBits(NUM_REGISTRADOR-1);

    binario = ((int64_t)inst.opcode << (nbit_registrador + nbit_registrador + BITS_INTEIRO));

    switch(inst.opcode){
        case ADD:
        case SUB:
        case MULT:
        case DIV:
            binario |= ((int64_t)inst.dest << (nbit_registrador + BITS_INTEIRO));
            binario |= ((int64_t)inst.op1 << BITS_INTEIRO);
            binario |= ((int64_t)inst.op2 << (BITS_INTEIRO - nbit_registrador));
            break;
        case ADDI:
        case SUBI:
        case MULTI:
        case DIVI:
            binario |= ((int64_t)inst.dest << (nbit_registrador + BITS_INTEIRO));
            binario |= ((int64_t)inst.op1 << BITS_INTEIRO);
            binario |= LAST((int64_t)inst.op2, BITS_INTEIRO);
            break;
        case LD:
        case LI:
        case LUI:
            binario |= ((int64_t)inst.dest << (BITS_INTEIRO + nbit_registrador));
            binario |= LAST((int64_t)inst.op1, BITS_INTEIRO) << nbit_registrador;
            break;
        case SD:
            binario |= ((int64_t)inst.dest << (nbit_registrador + nbit_registrador));
            binario |= (int64_t)inst.op1 << nbit_registrador;
            break;
        case BEQ:
        case BNE:
        case BG:
        case BGE:
        case BL:
        case BLE:
            binario |= ((int64_t)inst.op1 << (nbit_registrador + BITS_INTEIRO));
            binario |= ((int64_t)inst.op2 << BITS_INTEIRO);
            binario |= LAST((int64_t)inst.dest, BITS_INTEIRO);
            break;
        case NOP: break;
    }

    return binario;
}

Instrucao binarioParaInstrucao(int64_t valor){
    Instrucao inst;
    int nbit_operacoes = calculaBits(NUM_OPERACOES-1);
    int nbit_registrador = calculaBits(NUM_REGISTRADOR-1);

    inst.opcode = LAST(valor >> (nbit_registrador + nbit_registrador + BITS_INTEIRO), nbit_operacoes);

    switch(inst.opcode){
        case ADD:
        case SUB:
        case MULT:
        case DIV:
            inst.dest = LAST(valor >> (nbit_registrador + BITS_INTEIRO), nbit_registrador);
            inst.op1 = LAST(valor >> BITS_INTEIRO, nbit_registrador);
            inst.op2 = LAST(valor >> (BITS_INTEIRO - nbit_registrador), nbit_registrador);
            break;
        case ADDI:
        case SUBI:
        case MULTI:
        case DIVI:
            inst.dest = LAST(valor >> (nbit_registrador + BITS_INTEIRO), nbit_registrador);
            inst.op1 = LAST(valor >> BITS_INTEIRO, nbit_registrador);
            inst.op2 = (int)LAST(valor, BITS_INTEIRO);
            break;
        case LD:
        case LI:
        case LUI:
            inst.dest = LAST(valor >> (BITS_INTEIRO + nbit_registrador), nbit_registrador);
            if (inst.opcode == LUI)
                inst.op1 = (unsigned)LAST(valor >> nbit_registrador, BITS_INTEIRO);
            else
                inst.op1 = (int)LAST(valor >> nbit_registrador, BITS_INTEIRO);
            inst.op2 = LAST(valor, nbit_registrador);
            break;
        case SD:
            inst.dest = LAST(valor >> (nbit_registrador + nbit_registrador), BITS_INTEIRO);
            inst.op1 = LAST(valor >> nbit_registrador, nbit_registrador);
            inst.op2 = LAST(valor, nbit_registrador);
            break;
        case BEQ:
        case BNE:
        case BG:
        case BGE:
        case BL:
        case BLE:
            inst.op1 = LAST(valor >> (nbit_registrador + BITS_INTEIRO), nbit_registrador);
            inst.op2 = LAST(valor >> BITS_INTEIRO, nbit_registrador);
            inst.dest = (int)LAST(valor, BITS_INTEIRO);
            break;
        case NOP: break;
    }

    return inst;
}