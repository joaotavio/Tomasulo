#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
#include "util.h"
#include "tomasulo.h"
#include "instrucao.h"

#define MAX_TAM_LINHA 100

bool decodificaComponente(char str[], int valor){ //MUDAR NOME
    strMinuscula(str);

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
    else if (wcsncmp(wstr, L"divisor", MAX_TAM_LINHA) == 0){
        qtd_divisor = valor;
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

    fscanf(arquivo, " %100[^\n]", buffer);
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

    fscanf(arquivo, " %100[^\n]", buffer);
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
    if (!lerCabecalhoArquitetura(arquivo))
        return false;

    if (!lerCabecalhoCiclos(arquivo))
        return false;

    return true;
}

Instrucao decodificaString(char str[]){ //mudar nome
    Instrucao inst = {0};
    char *opcode;
    char *operandos[3];
    char str_copia[MAX_TAM_LINHA];

    strcpy(str_copia, str);

    opcode = strtok(str_copia, " ");
    strMinuscula(opcode);
    
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

    fscanf(arquivo, " %100[^\n]", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "texto:") != 0)
        return false;
    
    printf("\n%s\n", buffer);

    while (!feof(arquivo)){
        if (fscanf(arquivo, " %100[^\n]", buffer) != 1)
            return true;

        inst = decodificaString(buffer);
        if (inst.opcode == OP_INVALIDO)
            return false;
        
        insereMemoria(memoria, buffer);

        printf("%s\n", buffer);
        //printf("%d %s %s %s\n", inst.opcode, inst.op1, inst.op2, inst.op3);
    }

    return true;
}

bool lerArquivo(FILE* arquivo){
    if (!lerCabecalho(arquivo)){
        printf("ERRO: Cabecalho do arquivo invalido.\n");
        return false;
    }

    if (!lerInstrucoes(arquivo)){
        printf("ERRO: Instrucao invalida.\n");
        return false;
    }

    return true;
}