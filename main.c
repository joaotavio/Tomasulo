#include <stdio.h>
#include <stdbool.h>
#include <locale.h>
#include "arquivo.h"
#include "tomasulo.h"

void printCiclo(){
    //printa oq esta acontecendo em cada ciclo
    //tentar fazer em forma de tabela
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
}

void iniciar(){
    FILE* arquivo;
    arquivo = fopen("teste.txt", "r");

    if (!lerArquivo(arquivo)){
        printf("\nERRO: Arquivo esta em um formato incorreto.\n");
        fclose(arquivo);
        return;
    }
    fclose(arquivo);

    iniciarComponentes();

    //FAZER EXECUCAO AQUI
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL,"");

    iniciar();

    getchar();
    return 0;
}