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
    freeFila(fila);
}

void iniciarComponentes(){
    inicializaFila(tam_fila);
    inicializaER(&er_somador, &somador);
    inicializaER(&er_multiplicador, &multiplicador);
    inicializaBuffer(&load);
    inicializaBuffer(&store);
    inicializaRegistrador(registrador);
    inicializaBarramento(&barramento, load.tamMax, store.tamMax, somador.tamMax, multiplicador.tamMax);
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

    iniciarComponentes();
    iniciarTomasulo();
    liberarComponentes();

    printf("Execucao terminada...\n");
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL,"");

    iniciar();

    getchar();
    return 0;
}