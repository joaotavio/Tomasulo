#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include "arquivo.h"
#include "tomasulo.h"
#include "instrucao.h"
#include "fila.h"
//#include "memoria.h"

#define NOME_ARQ_ENTRADA "entrada.txt"
#define NOME_ARQ_SAIDA "saida.txt"

void printCiclo(){
    
}

void printRegistrador(){

}

void liberarComponentes(){
    free(memoria);
    free(est_somador);
    free(est_multiplicador);
    free(somador);
    free(multiplicador);
    free(load);
    free(store);
    free(janela.inst);
}

void iniciarComponentes(){
    inicializaJanela(tam_janela);
    inicializaER(est_somador, somador, qtd_somador);
    inicializaER(est_multiplicador, multiplicador, qtd_multiplicador);
    inicializaBuffer(load, qtd_buffer_carga);
    inicializaBuffer(store, qtd_buffer_escrita);
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