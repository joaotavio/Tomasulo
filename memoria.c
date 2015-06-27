#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "memoria.h"
#include "tomasulo.h"

#define BITS_INTEIRO 32
#define LAST(k,n) ((k) & ((1<<(n))-1))

int64_t memoria[TAM_MEMORIA];

int numBits(int64_t valor){
    if (valor < 0)
        return -1;

    if (valor == 0)
        return 1;

    return (int)floor(log2(valor))+1;
}

void binarioParaString(int64_t valor, char str[]){
    int i = 0;

    if (valor == 0){
        strcpy(str, "0");
        return;
    }
    if (valor < 0){
        valor = -valor;
        str[0] = '-';
        i++;
    }

    i += numBits(valor);
    str[i] = '\0';
    while (valor > 0) {
        i--;
        if (valor & 1)
            str[i] = '1';
        else
            str[i] = '0';
        valor >>= 1;
    }
}

void printBinario(int64_t valor){
    char str[64];
    binarioParaString(valor, str);
    printf("%s\n", str);
}

int64_t instrucaoParaBinario(Instrucao inst){
    //talvez nao funcione com imediato negativo TESTAR ISSO
    int64_t binario;
    int nbit_operacoes = numBits(NUM_OPERACOES-1);
    int nbit_dest = numBits(NUM_REGISTRADOR-1);
    int nbit_op1 = nbit_dest;

    if (inst.opcode == LD)
        nbit_op1 = BITS_INTEIRO - 1;
    else if (inst.opcode == SD)
        nbit_dest = BITS_INTEIRO - 1;

    binario = (int64_t)inst.opcode;

    binario |= ((int64_t)inst.dest << nbit_operacoes);

    binario |= ((int64_t)inst.op1 << (nbit_dest + nbit_operacoes));

    binario |= ((int64_t)inst.op2 << (nbit_dest + nbit_op1 + nbit_operacoes));

    return binario;
}

Instrucao binarioParaInstrucao(int64_t valor){
    Instrucao inst;
    int nbit_operacoes = numBits(NUM_OPERACOES-1);
    int nbit_dest = numBits(NUM_REGISTRADOR-1);
    int nbit_op1 = nbit_dest;

    inst.opcode = LAST(valor, nbit_operacoes);
    valor >>= nbit_operacoes;

    if (inst.opcode == LD)
        nbit_op1 = BITS_INTEIRO - 1;
    else if (inst.opcode == SD)
        nbit_dest = BITS_INTEIRO - 1;

    inst.dest = LAST(valor, nbit_dest);
    valor >>= nbit_dest;

    inst.op1 = LAST(valor, nbit_op1);
    valor >>= nbit_op1;

    inst.op2 = valor;

    return inst;
}

//instrucao insere no final da memoria
bool memoriaInsereInst(Instrucao inst, int posicao){
    if (posicao < 0 || posicao >= TAM_MEMORIA){
        printf("ERRO: Posicao de memoria invalida.\n");
        return false;
    }

    posicao = TAM_MEMORIA - 1 - posicao;
    memoria[posicao] = instrucaoParaBinario(inst);

    return true;
}

bool memoriaInsereDado(int valor, int posicao){
    if (posicao < 0 || posicao >= TAM_MEMORIA){
        printf("ERRO: Posicao de memoria invalida.\n");
        return false;
    }

    memoria[posicao] = valor;

    return true;
}

bool memoriaObterInst(int posicao, Instrucao *inst_retorno){
    if (posicao < 0 || posicao >= TAM_MEMORIA){
        printf("ERRO: Posicao de memoria invalida.\n");
        return false;
    }

    *inst_retorno = binarioParaInstrucao(memoria[TAM_MEMORIA - 1 - posicao]);
    return true;
}

bool memoriaObterDado(int posicao, int *valor_retorno){
    if (posicao < 0 || posicao >= TAM_MEMORIA){
        printf("ERRO: Posicao de memoria invalida.\n");
        return false;
    }

    //verificar problema de posicao ser um binario de instrucao
    *valor_retorno = memoria[posicao];
    return true;
}

void printMemoria(){
    char str[64] = "";
    int i;
    printf("\nMEMORIA:\n");
    for (i = 0; i < TAM_MEMORIA; i++){
        binarioParaString(memoria[i], str);
        printf("[%d]: BINARIO - %s\n", i, str);
    }
}