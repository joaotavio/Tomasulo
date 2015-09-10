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
    FILE* arq_entrada;
    FILE* arq_saida;
    arq_entrada = fopen(NOME_ARQ_ENTRADA, "r");
    arq_saida = fopen(NOME_ARQ_SAIDA, "w");

    if (!lerArquivo(arq_entrada)){
        printf("\nERRO: Arquivo esta em um formato incorreto.\n");
        fclose(arq_entrada);
        return;
    }
    fclose(arq_entrada);

    iniciarComponentes();
    iniciarTomasulo(arq_saida);
    liberarComponentes();

    fclose(arq_saida);
    printf("Execucao terminada com sucesso...\n");
    printf("Arquivo de saida: saida.txt\n");    
}

int main(int argc, char const *argv[]) {

    iniciar();

    printf("\nPressione qualquer tecla para continuar...\n");
    getchar();
    return 0;
}