#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>
#include "util.h"
#include "tomasulo.h"
#include "instrucao.h"
#include "memoria.h"

#define MAX_TAM_LINHA 50

/*void lerLinha(char buffer[], FILE* arquivo){
    fgets(buffer, MAX_TAM_LINHA, arquivo);

    while (isspace(buffer[0]))
        fgets(buffer, MAX_TAM_LINHA, arquivo);

    int tamanho = strlen(buffer);

    while (isspace(buffer[tamanho - 1]))
        buffer[--tamanho] = '\0';
}*/

bool decodificaComponente(char str[], int valor){ //MUDAR NOME
    strMinuscula(str);

    mbstate_t ss1;
    memset(&ss1, '\0', sizeof(ss1));
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

//talvez fazer funcao generalizada para ler cabecalho
bool lerCabecalhoArquitetura(FILE* arquivo){
    char buffer[MAX_TAM_LINHA];
    int valor;
    int tamanho;

    fscanf(arquivo, " %100[^\n\r ]", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "arquitetura:") != 0)
        return false;
    
    printf("%s\n", buffer);

    int i;
    for (i = 0; i < NUM_COMPONENTES; ++i) {
        if (fscanf(arquivo, " %100[^0-9] %d", buffer, &valor) != 2)
            return false;
        tamanho = strlen(buffer);

        while (isblank(buffer[tamanho - 1]))
            buffer[--tamanho] = '\0';

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
    int tamanho;

    fscanf(arquivo, " %100[^\n\r ]", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "ciclos:") != 0)
        return false;
    
    printf("\n%s\n", buffer);

    int i;
    for (i = 0; i < NUM_OPERACOES; ++i) {
        if (fscanf(arquivo, " %100[^0-9] %d", buffer, &valor) != 2)
            return false;

        tamanho = strlen(buffer);

        while (isblank(buffer[tamanho - 1]))
            buffer[--tamanho] = '\0';

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

//MUDAR ESSE NOME TAMBEM
bool decodificaOperando(char str[], int *retorno, bool imediato){
    int i;
    char numero[strlen(str)];
    if (imediato){
        if (!isNumero(str))
            return false;
        
        *retorno = atoi(str);
    }
    else {
        if (str[0] == 'R' || str[0] == 'r'){
            for (i = 0; i < strlen(str); i++){
                numero[i] = str[i+1];
            }
            numero[i] = '\0';
            
            if (!isNumero(numero))
                return false;

            *retorno = atoi(numero);

            if (*retorno < 0 || *retorno >= NUM_REGISTRADOR)
                return false;
        }
        else {
            return false;
        }
    }

    //se numero do registrador lido for maior que 32 retornar falso
    return true;
}

bool decodificaInstrucao(char str[], Instrucao *inst){ //mudar nome
    char *opcode;
    char *operandos[3];
    char str_copia[MAX_TAM_LINHA];

    strcpy(str_copia, str);

    opcode = strtok(str_copia, " ");
    strMinuscula(opcode);

    int i;
    for (i = 0; i < 3; i++)
        operandos[i] = strtok(NULL, " ,");

    if (strtok(NULL, " ,") != NULL)
        return false;

    if (operandos[0] != NULL && operandos[1] != NULL){
        if (operandos[2] != NULL){
            if (strcmp(opcode, "add") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, false))
                    return false;
                inst->opcode = ADD;
            }
            else if (strcmp(opcode, "addi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, true))
                    return false;
                inst->opcode = ADDI;
            }
            else if (strcmp(opcode, "sub") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, false))
                    return false;
                inst->opcode = SUB;
            }
            else if (strcmp(opcode, "subi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, true))
                    return false;
                inst->opcode = SUBI;
            }
            else if (strcmp(opcode, "mult") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, false))
                    return false;
                inst->opcode = MULT;
            }
            else if (strcmp(opcode, "multi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, true))
                    return false;
                inst->opcode = MULTI;
            }
            else if (strcmp(opcode, "div") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, false))
                    return false;
                inst->opcode = DIV;
            }
            else if (strcmp(opcode, "divi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, true))
                    return false;
                inst->opcode = DIVI;
            }
            else if (strcmp(opcode, "beq") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, true))
                    return false;
                inst->opcode = BEQ;
            }
            else if (strcmp(opcode, "bne") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2, true))
                    return false;
                inst->opcode = BNE;
            }
            else {
                return false;
            }
        }
        else {
            if (strcmp(opcode, "ld") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, false))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, true))
                    return false;
                if (inst->op1 < 0)
                    return false;
                inst->opcode = LD;
            }
            else if (strcmp(opcode, "sd") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest, true))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1, false))
                    return false;
                if (inst->dest < 0)
                    return false;
                inst->opcode = SD;
            }
            else {
                return false;
            }
        }
    }
    else {
        return false;
    }
    
    return true;
}

bool lerInstrucoes(FILE* arquivo){
    char buffer[MAX_TAM_LINHA];
    Instrucao inst;

    fscanf(arquivo, " %100[^\n\r ]", buffer);
    strMinuscula(buffer);
    if (strcmp(buffer, "texto:") != 0)
        return false;
    
    printf("\n%s\n", buffer);

    int i = 0;
    while (!feof(arquivo)){
        inst = (Instrucao){0};
        if (fscanf(arquivo, " %100[^\n\r]", buffer) != 1)
            return true;

        if (!decodificaInstrucao(buffer, &inst))
            return false;
        
        memoriaInsereInst(inst, i);

        //printf("%s\n", buffer);
        printf("%d R%d, R%d, R%d\n", inst.opcode, inst.dest, inst.op1, inst.op2);
        i++;
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