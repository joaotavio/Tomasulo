#include "util.h"
#include "tomasulo.h"
#include "instrucao.h"

#define BITS_INTEIRO 32
#define LAST(k,n) ((k) & ((1<<(n))-1))

int64_t instrucaoParaBinario(Instrucao inst){
    //talvez nao funcione com imediato negativo TESTAR ISSO
    //USAR PROVAVELMENTE BITS_INTEIRO +1 POR CAUSA DO INT MIN Q PRECISA DE 32
    int64_t binario;
    int nbit_dest = calculaBits(NUM_REGISTRADOR-1);
    int nbit_op1 = nbit_dest;
    int nbit_op2 = BITS_INTEIRO;

    if (inst.opcode == LD){
        nbit_op2 = nbit_op1; 
        nbit_op1 = BITS_INTEIRO;
    }
    else if (inst.opcode == SD){
        nbit_op2 = nbit_dest;
        nbit_dest = BITS_INTEIRO;
    }

    //printBinario(INT_MAX);
    binario = ((int64_t)inst.opcode << (nbit_dest + nbit_op1 + nbit_op2));
    binario |= ((int64_t)inst.dest << (nbit_op1 + nbit_op2));
    binario |= ((int64_t)inst.op1 << nbit_op2);

    if (inst.op2 < 0){
        inst.op2 = (~inst.op2) + 1;
        binario |= ((int64_t)1 << (BITS_INTEIRO-1));
    }

    binario |= (int64_t)inst.op2;

    return binario;
}

Instrucao binarioParaInstrucao(int64_t valor){
    Instrucao inst;
    int nbit_operacoes = calculaBits(NUM_OPERACOES-1);
    int nbit_dest = calculaBits(NUM_REGISTRADOR-1);
    int nbit_op1 = nbit_dest;
    int nbit_op2 = BITS_INTEIRO;

    inst.opcode = LAST(valor >> (nbit_dest + nbit_op1 + nbit_op2), nbit_operacoes);

    if (inst.opcode == LD){
        nbit_op2 = nbit_op1; 
        nbit_op1 = BITS_INTEIRO;
    }
    else if (inst.opcode == SD){
        nbit_op2 = nbit_dest;
        nbit_dest = BITS_INTEIRO - 1;
    }

    inst.dest = LAST(valor >> (nbit_op1 + nbit_op2), nbit_dest);
    if (inst.opcode == LD)
        nbit_op1 = BITS_INTEIRO - 1;
    inst.op1 = LAST(valor >> nbit_op2, nbit_op1);
    inst.op2 = LAST(valor, nbit_op2 - 1);

    if (LAST(valor >> (nbit_op2 - 1), 1) == 1 && inst.opcode != LD && inst.opcode != SD)
        inst.op2 = (~inst.op2) + 1;

    return inst;
}