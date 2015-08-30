#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include "arquivo.h"
#include "tomasulo.h"
#include "instrucao.h"
#include "fila.h"

#define NOME_ARQ_ENTRADA "entrada.txt"
#define NOME_ARQ_SAIDA "saida.txt"

void liberarComponentes(){
    free(memoria);
    free(er_somador.est_reserva);
    free(er_multiplicador.est_reserva);
    free(somador.un_funcional);
    free(multiplicador.un_funcional);
    free(load.buffer);
    free(store.buffer);
    free(janela.inst);
}

void iniciarComponentes(){
    inicializaJanela();
    inicializaER(&er_somador, &somador);
    inicializaER(&er_multiplicador, &multiplicador);
    inicializaBuffer(&load);
    inicializaBuffer(&store);
}

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

    printInstrucao(memoriaObterInst(0));
    printInstrucao(memoriaObterInst(1));
    printInstrucao(memoriaObterInst(2));
    printInstrucao(memoriaObterInst(3));
    printInstrucao(memoriaObterInst(4));
    printInstrucao(memoriaObterInst(5));
    printInstrucao(memoriaObterInst(6));
    printInstrucao(memoriaObterInst(7));
    printInstrucao(memoriaObterInst(8));
    printInstrucao(memoriaObterInst(9));
    printInstrucao(memoriaObterInst(10));
    printInstrucao(memoriaObterInst(11));
    printInstrucao(memoriaObterInst(12));
    int dado = memoriaObterDado(0);
    printf("\nDADO: %d\n", dado);
    dado = memoriaObterDado(1);
    printf("DADO: %d\n", dado);
    dado = memoriaObterDado(2);
    printf("DADO: %d\n\n", dado);


    iniciarComponentes();

    iniciarTomasulo();

    liberarComponentes();
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL,"");

    iniciar();

    getchar();
    return 0;
}