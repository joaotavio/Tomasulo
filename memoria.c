#include <stdio.h>
#include <stdint.h>
#include "memoria.h"
#include "tomasulo.h"
#include "util.h"

int64_t memoria[TAM_MEMORIA];

bool fimDeInstrucao(int posicao){
    return memoria[posicao] == FIM_DE_INSTRUCAO;
}

void memoriaInsereInst(Instrucao inst, int posicao){
    memoria[posicao] = instrucaoParaBinario(inst);
    memoria[posicao+1] = FIM_DE_INSTRUCAO;
}

void memoriaInsereDado(int valor, int posicao){
    memoria[posicao] = valor;
}

Instrucao memoriaObterInst(int posicao){
    return binarioParaInstrucao(memoria[posicao]);
}

int memoriaObterDado(int posicao){
    return memoria[posicao];
}

void printMemoria(){
    char str[64] = "";
    int i;
    printf("\nMEMORIA:\n");
    for (i = 0; i < TAM_MEMORIA; i++){
        binarioParaString(memoria[i], str);
        printf("[%d]: - %s\n", i, str);
    }
}