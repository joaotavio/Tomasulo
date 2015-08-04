#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include "arquivo.h"
#include "tomasulo.h"
#include "instrucao.h"

#define NOME_ARQ_ENTRADA "entrada.txt"
#define NOME_ARQ_SAIDA "saida.txt"

void printCiclo(){
    
}

void printRegistrador(){

}

void destruirComponentes(){
    freeComponente(fila);
    freeComponente(janela);
    freeComponente(load);
    freeComponente(store);
    freeComponente(somador);
    freeComponente(multiplicador);
    freeComponente(divisor);
}

void iniciarComponentes(){
    fila = criaComponente(qtd_busca_inst);
    janela = criaComponente(qtd_janela_inst);
    load = criaComponente(qtd_buffer_carga);
    store = criaComponente(qtd_buffer_escrita);
    somador = criaComponente(qtd_somador);
    multiplicador = criaComponente(qtd_multiplicador);
    divisor = criaComponente(qtd_divisor);
    //cria estacoes reserva
}
#include "memoria.h"
void iniciar(){
    //lembrar de quando tentar abrir arquivo q nao existe tratar erro
    FILE* arquivo;
    arquivo = fopen("teste.txt", "r");

    if (!lerArquivo(arquivo)){
        printf("\nERRO: Arquivo esta em um formato incorreto.\n");
        fclose(arquivo);
        return;
    }
    fclose(arquivo);

    printMemoria();

    Instrucao inst;
    inst = binarioParaInstrucao(memoria[0]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[1]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[2]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[3]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[4]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[5]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[6]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[7]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
    inst = binarioParaInstrucao(memoria[8]);
    printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);

    iniciarComponentes();

    //FAZER EXECUCAO AQUI
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL,"");

    iniciar();

    getchar();
    return 0;
}