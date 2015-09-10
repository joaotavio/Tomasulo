#include <stdio.h>
#include <stdint.h>
#include "memoria.h"
#include "tomasulo.h"
#include "util.h"

MemoriaExec mem_exec;
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

void memoriaExecInsere(MemoriaExec *mem_exec, int id, Operacoes opcode, int origem, int destino, int ciclos){
    mem_exec->id = id;
    mem_exec->opcode = opcode;
    mem_exec->origem = origem;
    mem_exec->destino = destino;
    mem_exec->ciclos = ciclos;
    mem_exec->busy = true;
}

void memoriaExecRemove(MemoriaExec *mem_exec){
    mem_exec->id = -1;
    mem_exec->opcode = NOP;
    mem_exec->origem = 0;
    mem_exec->destino = 0;
    mem_exec->ciclos = 0;
    mem_exec->busy = false;
}

char* memToString(MemoriaExec mem_exec){
    char *str = malloc(sizeof(char)*MAX_STR_PRINT);
    switch(mem_exec.opcode){
        case LD:
            sprintf(str, "ld R%d, [%d]", mem_exec.destino, mem_exec.origem);
            break;
        case SD:
            sprintf(str, "sd [%d], %d", mem_exec.destino, mem_exec.origem);
            break;
        default:
            break;
    }

    return str;
}
