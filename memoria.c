#include <stdio.h>
#include <stdint.h>
#include "memoria.h"
#include "tomasulo.h"
#include "util.h"

int64_t memoria[TAM_MEMORIA];

bool memoriaInsereInst(Instrucao inst, int posicao){
    if (posicao < 0 || posicao >= TAM_MEMORIA){
        printf("ERRO: Posicao de memoria invalida.\n");
        return false;
    }

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