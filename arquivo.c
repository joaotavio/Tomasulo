#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "util.h"
#include "tomasulo.h"
#include "instrucao.h"
#include "memoria.h"

#define MAX_TAM_LINHA 50

bool decodificaComponente(FILE* arquivo, char str[], int valor){ //MUDAR NOME
    strMinuscula(str);

    if (strcmp(str, "somador") == 0){
        er_somador.tamMax = valor;
    }
    else if (strcmp(str, "multiplicador") == 0){
        er_multiplicador.tamMax = valor;
    }
    else if (strcmp(str, "busca de instrucoes") == 0){
        qtd_busca_inst = valor;
    }
    else if (strcmp(str, "janela de instrucoes") == 0){
        janela.tamMax = valor;
    }
    else if (strcmp(str, "buffer de carga") == 0){
        load.tamMax = valor;
    }
    else if (strcmp(str, "buffer de escrita") == 0){
        store.tamMax = valor;
    }
    else if (strcmp(str, "emissao") == 0){
        qtd_emissao = valor;
    }
    else if (strcmp(str, "memoria") == 0){
        tam_memoria = valor;
    }
    else if (strcmp(str, "imemoria") == 0){
        intervalo_mem_x = valor;
        if (fscanf(arquivo, " ,%d", &valor) != 1)
            return false;
        intervalo_mem_y = valor;
        printf("  imemoria -- %d, %d\n", intervalo_mem_x, intervalo_mem_y);
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
        if (fscanf(arquivo, " %100[^=] %*[^0-9] %d", buffer, &valor) != 2)
            return false;
                
        tamanho = strlen(buffer);

        while (isspace(buffer[tamanho - 1]))
            buffer[--tamanho] = '\0';

        if (!decodificaComponente(arquivo, buffer, valor))
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
    else if (strcmp(str, "addi") == 0){
        ciclo_addi = valor;
    }
    else if (strcmp(str, "sub") == 0){
        ciclo_sub = valor;
    }
    else if (strcmp(str, "subi") == 0){
        ciclo_subi = valor;
    }
    else if (strcmp(str, "mult") == 0){
        ciclo_mult = valor;
    }
    else if (strcmp(str, "multi") == 0){
        ciclo_multi = valor;
    }
    else if (strcmp(str, "div") == 0){
        ciclo_div = valor;
    }
    else if (strcmp(str, "divi") == 0){
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
    else if (strcmp(str, "bl") == 0){
        ciclo_bl = valor;
    }
    else if (strcmp(str, "ble") == 0){
        ciclo_ble = valor;
    }
    else if (strcmp(str, "bg") == 0){
        ciclo_bg = valor;
    }
    else if (strcmp(str, "bge") == 0){
        ciclo_bge = valor;
    }
    else if (strcmp(str, "li") == 0){
        ciclo_li = valor;
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
    //NUM_OPERACOES - 3 POR CAUSA DA OPERACAO NOP, jump e exit
    for (i = 0; i < NUM_OPERACOES - 3; ++i) {
        if (fscanf(arquivo, " %100[^=] %*[^0-9] %d", buffer, &valor) != 2)
            return false;

        tamanho = strlen(buffer);

        while (isspace(buffer[tamanho - 1]))
            buffer[--tamanho] = '\0';

        if (!decodificaCiclo(buffer, valor))
            return false;
        printf("  %s --- %d\n", buffer, valor);
    }

    return true;
}

bool decodificaImediato(char str[], int *retorno){
    if (!isNumero(str))
        return false;
    
    *retorno = atoll(str);
    return true;
}

//MUDAR ESSE NOME TAMBEM
bool decodificaOperando(char str[], int *retorno){
    char numero[strlen(str)];
    if (sscanf(str, "r%s", numero) == 1){
        if (!isNumero(numero))
            return false;

        *retorno = atoll(numero);

        if (*retorno < 0 || *retorno >= NUM_REGISTRADOR)
            return false;
    }
    else 
        return false;    

    return true;
}

bool decodificaInstrucao(char str[], Instrucao *inst){ //mudar nome
    strMinuscula(str);
    char *opcode;
    char *operandos[3];
    char str_copia[MAX_TAM_LINHA];

    strcpy(str_copia, str);

    opcode = strtok(str_copia, " ");

    int i;
    for (i = 0; i < 3; i++)
        operandos[i] = strtok(NULL, " ,");

    if (strtok(NULL, " ,") != NULL)
        return false;

    if (operandos[0] != NULL && operandos[1] != NULL){
        if (operandos[2] != NULL){
            if (strcmp(opcode, "add") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2))
                    return false;
                inst->opcode = ADD;
            }
            else if (strcmp(opcode, "addi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->op2))
                    return false;
                inst->opcode = ADDI;
            }
            else if (strcmp(opcode, "sub") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2))
                    return false;
                inst->opcode = SUB;
            }
            else if (strcmp(opcode, "subi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->op2))
                    return false;
                inst->opcode = SUBI;
            }
            else if (strcmp(opcode, "mult") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2))
                    return false;
                inst->opcode = MULT;
            }
            else if (strcmp(opcode, "multi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->op2))
                    return false;
                inst->opcode = MULTI;
            }
            else if (strcmp(opcode, "div") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[2], &inst->op2))
                    return false;
                inst->opcode = DIV;
            }
            else if (strcmp(opcode, "divi") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->op2))
                    return false;
                inst->opcode = DIVI;
            }
            else if (strcmp(opcode, "beq") == 0){
                if (!decodificaOperando(operandos[0], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op2))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->dest))
                    return false;
                inst->opcode = BEQ;
            }
            else if (strcmp(opcode, "bne") == 0){
                if (!decodificaOperando(operandos[0], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op2))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->dest))
                    return false;
                inst->opcode = BNE;
            }
            else if (strcmp(opcode, "bl") == 0){
                if (!decodificaOperando(operandos[0], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op2))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->dest))
                    return false;
                inst->opcode = BL;
            }
            else if (strcmp(opcode, "ble") == 0){
                if (!decodificaOperando(operandos[0], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op2))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->dest))
                    return false;
                inst->opcode = BLE;
            }
            else if (strcmp(opcode, "bg") == 0){
                if (!decodificaOperando(operandos[0], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op2))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->dest))
                    return false;
                inst->opcode = BG;
            }
            else if (strcmp(opcode, "bge") == 0){
                if (!decodificaOperando(operandos[0], &inst->op1))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op2))
                    return false;
                if (!decodificaImediato(operandos[2], &inst->dest))
                    return false;
                inst->opcode = BGE;
            }
            else {
                return false;
            }
        }
        else {
            if (strcmp(opcode, "ld") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaImediato(operandos[1], &inst->op1))
                    return false;
                if (inst->op1 < 0)
                    return false;
                inst->opcode = LD;
            }
            else if (strcmp(opcode, "sd") == 0){
                if (!decodificaImediato(operandos[0], &inst->dest))
                    return false;
                if (!decodificaOperando(operandos[1], &inst->op1))
                    return false;
                if (inst->dest < 0)
                    return false;
                inst->opcode = SD;
            }
            else if (strcmp(opcode, "li") == 0){
                if (!decodificaOperando(operandos[0], &inst->dest))
                    return false;
                if (!decodificaImediato(operandos[1], &inst->op1))
                    return false;
                inst->opcode = LI;
            }
            else {
                return false;
            }
        }
    }
    else if (operandos[0] != NULL){
        if (strcmp(opcode, "jump") == 0){
            if (!decodificaImediato(operandos[0], &inst->dest))
                return false;
            inst->opcode = JUMP;
        }
        else {
            return false;
        }
    }
    else if (strcmp(opcode, "exit") == 0){
        inst->opcode = EXIT;
    }
    else if (strcmp(opcode, "nop") == 0){
        inst->opcode = NOP;
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

        strMinuscula(buffer);
        if (strcmp(buffer, "dados:") == 0)
            return true;

        if (!decodificaInstrucao(buffer, &inst))
            return false;
        
        memoriaInsereInst(inst, i);

        printInstrucao(inst);
        i++;
    }

    return true;
}

bool lerDados(FILE* arquivo){
    int num;
    int num_scanf;
    int i = 0;
    printf("\nDados:\n");
    while (!feof(arquivo)){
        num_scanf = fscanf(arquivo, " %d", &num);
        if (num_scanf < 0)
            return true;
        if (num_scanf != 1)
            return false;
        printf("NUM: %d\n", num);
        memoriaInsereDado(num, i);
        i++;
    }
    return true;
}

bool lerArquivo(FILE* arquivo){
    if (!lerCabecalhoArquitetura(arquivo)){
        printf("ERRO: Cabecalho de Arquitetura do arquivo invalida.\n");
        return false;
    }

    if (!lerCabecalhoCiclos(arquivo)){
        printf("ERRO: Cabecalho de Ciclos do arquivo invalido.\n");
        return false;
    }

    inicializaMemoria(tam_memoria);

    if (!lerInstrucoes(arquivo)){
        printf("ERRO: Instrucao invalida.\n");
        return false;
    }

    if (!lerDados(arquivo)){
        printf("ERRO: Dados invalidos.\n");
        return false;
    }

    return true;
}