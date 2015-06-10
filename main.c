#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include <ctype.h>
#include "fila.h"

#define NUM_OPERACOES 12
#define NUM_COMPONENTES 6 //MUDAR
#define MAX_TAM_LINHA 512

typedef enum operacoes {OP_INVALIDO, ld, sd, beq, bne, add, addi, sub, subi, mult, multi, Div, divi} Operacoes;

typedef struct instrucao {
    Operacoes opcode;
    char *op1; //MUDAR ESSES NOMES e mudar o tipo
    char *op2;
    char *op3;
} Instrucao;

/*** VARIÁVEIS GLOBAIS ***/

int qtd_somador;
int qtd_multiplicador;
int qtd_busca_inst;
int qtd_janela_inst;
int qtd_buffer_carga;
int qtd_buffer_escrita;

int ciclo_add;
int ciclo_addi;
int ciclo_sub;
int ciclo_subi;
int ciclo_mult;
int ciclo_multi;
int ciclo_div;
int ciclo_divi;
int ciclo_beq;
int ciclo_bne;
int ciclo_ld;
int ciclo_sd;

int cont_ciclos;

void strMinuscula(char str[]){
    int i;
    for (i = 0; i < strlen(str); ++i)
        str[i] = tolower(str[i]);
}

bool decodificaComponente(char str[], int valor){ //MUDAR NOME
    strMinuscula(str);

    //CONVERTE CHAR STR PARA WIDE CHAR PARA COMPARAR COM ACENTOS
    mbstate_t ss1 = 0;
    wchar_t wstr[MAX_TAM_LINHA];
    const char *pmbs1 = str;
    mbsrtowcs(wstr, &pmbs1, MAX_TAM_LINHA, &ss1);

    if (wcsncmp(wstr, L"somador", MAX_TAM_LINHA) == 0){
        qtd_somador = valor;
    }
    else if (wcsncmp(wstr, L"multiplicador", MAX_TAM_LINHA) == 0){
        qtd_multiplicador = valor;
    }
    else if (wcsncmp(wstr, L"busca de instruções", MAX_TAM_LINHA) == 0){
        qtd_busca_inst = valor;
    }
    else if (wcsncmp(wstr, L"janela de instruções", MAX_TAM_LINHA) == 0){
        qtd_janela_inst = valor;
    }
    else if (wcsncmp(wstr, L"buffer de carga", MAX_TAM_LINHA) == 0){
        qtd_buffer_carga = valor;
    }
    else if (wcsncmp(wstr, L"buffer de escrita", MAX_TAM_LINHA) == 0){
        qtd_buffer_escrita = valor;
    }
    else {
        return false;
    }

    return true;
}

bool lerCabecalhoArquitetura(FILE* arquivo){
    char buffer[MAX_TAM_LINHA];
    int valor;

    fscanf(arquivo, "%s", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "arquitetura:") != 0)
        return false;
    
    printf("%s\n", buffer);

    int i;
    for (i = 0; i < NUM_COMPONENTES; ++i) {
        if (fscanf(arquivo, " %100[^0-9] %d", buffer, &valor) != 2)
            return false;
        buffer[strlen(buffer)-1] = '\0';
        if (!decodificaComponente(buffer, valor))
            return false;
        printf("  %s --- %d\n", buffer, valor);
    }

    return true;
}

bool decodificaCiclo(char str[], int valor){ //MUDAR NOME
    strMinuscula(str);

    if (strcmp(str, "add") == 0){
        ciclo_add = valor;
    }
    else if (strcmp(str, "add.i") == 0){
        ciclo_addi = valor;
    }
    else if (strcmp(str, "sub") == 0){
        ciclo_sub = valor;
    }
    else if (strcmp(str, "sub.i") == 0){
        ciclo_subi = valor;
    }
    else if (strcmp(str, "mult") == 0){
        ciclo_mult = valor;
    }
    else if (strcmp(str, "mult.i") == 0){
        ciclo_multi = valor;
    }
    else if (strcmp(str, "div") == 0){
        ciclo_div = valor;
    }
    else if (strcmp(str, "div.i") == 0){
        ciclo_divi = valor;
    }
    else if (strcmp(str, "ld") == 0){
        ciclo_ld = valor;
    }
    else if (strcmp(str, "sd") == 0){
        ciclo_sd = valor;
    }
    else if (strcmp(str, "beq") == 0){
        ciclo_beq = valor;
    }
    else if (strcmp(str, "bne") == 0){
        ciclo_bne = valor;
    }
    else {
        return false;
    }

    return true;
}

bool lerCabecalhoCiclos(FILE* arquivo){
    char buffer[MAX_TAM_LINHA];
    int valor;

    fscanf(arquivo, "%s", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "ciclos:") != 0)
        return false;
    
    printf("\n%s\n", buffer);

    int i;
    for (i = 0; i < NUM_OPERACOES; ++i) {
        if (fscanf(arquivo, " %100[^0-9] %d", buffer, &valor) != 2)
            return false;
        buffer[strlen(buffer)-1] = '\0';
        if (!decodificaCiclo(buffer, valor))
            return false;
        printf("  %s --- %d\n", buffer, valor);
    }

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

bool fimArquivo(FILE* arq){
    long offset = ftell(arq);
    if (fgetc(arq) == EOF){
        fseek(arq, offset, SEEK_SET);
        return true;
    }

    fseek(arq, offset, SEEK_SET);
    return false;
}

Instrucao decodificaString(char str[]){ //mudar nome
    //entrada é uma linha do arquivo, decodifica e transforma em instrucao (se linha nao estivar no formado retorna erro)
    Instrucao inst = {0};
    char *opcode;
    char *operandos[3];

    opcode = strtok(str, " ");

    int i;
    for (i = 0; i < 3; i++)
        operandos[i] = strtok(NULL, " ,");

    if (operandos[0] != NULL && operandos[1] != NULL){
        if (operandos[2] != NULL){
            if (strcmp(opcode, "add") == 0){
                inst.opcode = add;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "add.i") == 0){
                inst.opcode = addi;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "sub") == 0){
                inst.opcode = sub;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "sub.i") == 0){
                inst.opcode = subi;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "mult") == 0){
                inst.opcode = mult;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "mult.i") == 0){
                inst.opcode = multi;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "div") == 0){
                inst.opcode = Div;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "div.i") == 0){
                inst.opcode = divi;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "beq") == 0){
                inst.opcode = beq;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "bne") == 0){
                inst.opcode = bne;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
        }
        else {
            if (strcmp(opcode, "ld") == 0){
                inst.opcode = ld;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
            else if (strcmp(opcode, "sd") == 0){
                inst.opcode = sd;
                inst.op1 = operandos[0];
                inst.op2 = operandos[1];
                inst.op3 = operandos[2];
            }
        }
    }
    
    return inst;
}

bool lerInstrucoes(FILE* arquivo){
    char buffer[MAX_TAM_LINHA];
    Instrucao inst;
    fscanf(arquivo, "%s", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "texto:") != 0)
        return false;
    
    printf("\n%s\n", buffer);

    //arrumar quando tem uma linha vazia no final
    while (!fimArquivo(arquivo)){
        if (fscanf(arquivo, " %100[^\n]", buffer) != 1)
            return false;
        inst = decodificaString(buffer);
        if (inst.opcode == OP_INVALIDO)
            return false;
    
        printf("%d %s %s %s\n", inst.opcode, inst.op1, inst.op2, inst.op3);
    }

    return true;
}

void iniciar(){
    FILE* arquivo;
    arquivo = fopen("teste.txt", "r");

    if (!lerCabecalho(arquivo)){
        printf("Cabecalho do arquivo invalido.\n");
    }

    //loop para ir lendo as linhas de instrução e ir executando
    if (!lerInstrucoes(arquivo)){
        printf("Instrucao invalida.\n");
    }

    fclose(arquivo);
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL,"");
    
    iniciar();

    getchar();
    return 0;
}