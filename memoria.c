#include <stdio.h>
#include <stdint.h>
#include "memoria.h"
#include "tomasulo.h"
#include "util.h"

uint8_t* memoria;
int tam_memoria;

void inicializaMemoria(int tamanho){
    tam_memoria = tamanho;
    memoria = (uint8_t*)calloc(tam_memoria, sizeof(uint8_t));
}

int calcularEndereco(int posicao){
    return posicao*4;
}

void memoriaInsereInst(Instrucao inst, int posicao){
    inst_t inst_binario = instrucaoParaBinario(inst);
    posicao = calcularEndereco(posicao);
    memoria[posicao] = LAST(inst_binario >> (BITS_INSTRUCAO - 8), 8);
    posicao++;
    memoria[posicao] = LAST(inst_binario >> (BITS_INSTRUCAO - 16), 8);
    posicao++;
    memoria[posicao] = LAST(inst_binario >> (BITS_INSTRUCAO - 24), 8);
    posicao++;
    memoria[posicao] = LAST(inst_binario >> (BITS_INSTRUCAO - 32), 8);
}

void memoriaInsereDado(int valor, int posicao){
    posicao = tam_memoria - calcularEndereco(posicao) - 1;
    memoria[posicao] = LAST(valor >> 24, 8);
    posicao--;
    memoria[posicao] = LAST(valor >> 16, 8);
    posicao--;
    memoria[posicao] = LAST(valor >> 8, 8);
    posicao--;
    memoria[posicao] = LAST(valor, 8);
}

Instrucao memoriaObterInst(int posicao){
    inst_t inst_binario;
    posicao = calcularEndereco(posicao);
    inst_binario = memoria[posicao] << 24;
    posicao++;
    inst_binario |= memoria[posicao] << 16;
    posicao++;
    inst_binario |= memoria[posicao] << 8;
    posicao++;
    inst_binario |= memoria[posicao];
    return binarioParaInstrucao(inst_binario);
}

int memoriaObterDado(int posicao){
    int valor;
    posicao = tam_memoria - calcularEndereco(posicao) - 1;
    valor = memoria[posicao] << 24;
    posicao--;
    valor |= memoria[posicao] << 16;
    posicao--;
    valor |= memoria[posicao] << 8;
    posicao--;
    valor |= memoria[posicao];
    return valor;
}

void printMemoria(){
    char str[64] = "";
    int i;
    printf("\nMEMORIA:\n");
    for (i = 0; i < tam_memoria; i++){
        binarioParaString(memoria[i], str);
        printf("[%d]: - %s\n", i, str);
    }
}