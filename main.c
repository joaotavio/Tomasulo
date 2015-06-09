#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include "fila.h"

typedef enum operacoes {ld, sd, beq, bne, add, addi, sub, subi, mult, multi, div, divi} Operacoes;

typedef struct instrucao {
    Operacoes opcode;
    char *op1;
    char *op2;
    char *op3;
} Instrucao;

Instrucao decodificaString(char str[]){ //mudar nome
    //entrada é uma linha do arquivo, decodifica e transforma em instrucao (se linha nao estivar no formado retorna erro)
    //usar sscanf
}

bool lerCabecalhoArquitetura(FILE* arquivo){
    char buffer[512];
    int x;

    fscanf(arquivo, "%s", buffer);
    if (strcmp(buffer, "ARQUITETURA:") != 0)
        return false;
    
    printf("%s\n", buffer);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    return true;
}

bool lerCabecalhoCiclos(FILE* arquivo){
    char buffer[512];
    int x;

    fscanf(arquivo, "%s", buffer);
    if (strcmp(buffer, "CICLOS:") != 0)
        return false;
    
    printf("\n%s\n", buffer);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    fscanf(arquivo, " %100[^0-9] %d", buffer, &x);
    buffer[strlen(buffer)-1] = '\0';
    printf("  %s --- %d\n", buffer, x);

    return true;
}

bool lerCabecalho(FILE* arquivo){
    //ler cabeçalho do arquivo (parametros)
    //retorna true se ler tudo certo e false se der erro

    if (!lerCabecalhoArquitetura(arquivo))
        return false;

    if (!lerCabecalhoCiclos(arquivo))
        return false;

    return true;
}

void iniciar(){
    FILE* arquivo;
    arquivo = fopen("teste.txt", "r");

    if (!lerCabecalho(arquivo)){
        printf("Cabecalho do arquivo invalido.\n");
    }

    //loop para ir lendo as linhas de instrução e ir executando

    fclose(arquivo);
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL,"");
    
    iniciar();

    getchar();
    return 0;
}