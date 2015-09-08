#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "tomasulo.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*** VARIÁVEIS GLOBAIS ***/

int tam_fila;
int qtd_busca_inst;
int qtd_emissao;

int intervalo_mem_x;
int intervalo_mem_y;

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
int ciclo_bl;
int ciclo_ble;
int ciclo_bg;
int ciclo_bge;
int ciclo_li;

int cont_ciclos;
int id;

/* REGISTRADORES ESPECÍFICOS */
int pc;
bool flag_jmp;

/* VARIÁVEIS PARA PRINT */

Instrucao *print_emitidas;
int num_emitidas;
char **print_er;
int num_print_er;
char **print_uf;
int num_print_uf;
char *print_ue;
int num_print_ue;
char *print_mem;
int num_print_mem;
char **print_bf;
int num_print_bf;
char **print_esc;
int num_print_esc;

/* FUNÇÕES PARA PRINT */
void printRegistrador(){
    int i;
    printf("REGISTRADORES: \n");
    for(i = 0; i < NUM_REGISTRADOR/2; i++){
        printf("R%d = %"PRId64" | ", i, registrador[i].valor);
    }
    printf("\n");
    for(i = NUM_REGISTRADOR/2; i < NUM_REGISTRADOR; i++){
        printf("R%d = %"PRId64" | ", i, registrador[i].valor);
    }
    printf("\n\n");
}

void printMemoria(){
    int i;
    printf("MEMORIA: \n");
    for (i = intervalo_mem_x; i <= intervalo_mem_y; ++i) {
        printf("[%d] = %d | ", i, memoriaObterDado(i));
    }
    printf("\n\n");
}

void printCiclo(){
    int i, num;
    char str[MAX_STR_PRINT];
    num = MAX(num_emitidas, MAX(num_print_er, MAX(num_print_uf, MAX(num_print_bf, MAX(num_print_ue, MAX(num_print_esc, num_print_mem))))));

    printf("\nCICLO: %d\n", cont_ciclos);
    printf("%-20s | %-20s | %-20s | %-20s | %-20s | %-20s | %-20s |\n", "EMISSAO", "UNIDADE DE ENDERECO", "BUFFER LOAD/STORE", "ESTACAO DE RESERVA", "UNIDADE FUNCIONAL", "MEMORIA", "ESCRITA");
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|----------------------|----------------------|\n");
    for (i = 0; i < num; ++i) {
        /* EMITIDAS */
        if (num_emitidas <= i)
            strcpy(str, "---");
        else
            strcpy(str, instToString(print_emitidas[i]));
        printf("%-20s | ", str);

        /* UNIDADE DE ENDEREÇO */
        if (num_print_ue <= i)
            strcpy(str, "---");     
        else
            strcpy(str, print_ue);
        printf("%-20s | ", str);

        /* BUFFER LOAD/STORE*/
        if (num_print_bf <= i)
            strcpy(str, "---");
        else
            strcpy(str, print_bf[i]);
        printf("%-20s | ", str);
            
        /* ESTAÇÃO DE RESERVA */
        if (num_print_er <= i)
            strcpy(str, "---");
        else
            strcpy(str, print_er[i]);
        printf("%-20s | ", str);

        /* UNIDADE FUNCIONAL */
        if (num_print_uf <= i)
            strcpy(str, "---");
        else
            strcpy(str, print_uf[i]);
        printf("%-20s | ", str);

        /* UNIDADE DE ENDEREÇO */
        if (num_print_mem <= i)
            strcpy(str, "---");     
        else
            strcpy(str, print_mem);
        printf("%-20s | ", str);

        /* ESCRITA */
        if (num_print_esc <= i)
            strcpy(str, "---");
        else
            strcpy(str, print_esc[i]);
        printf("%-20s | ", str);

        printf("\n");        
    }
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|----------------------|----------------------|\n\n");
    printRegistrador();
    printMemoria();
}

void criaVetorPrint(){
    print_emitidas = (Instrucao*)calloc(100, sizeof(Instrucao));
    print_er = (char**)calloc(er_somador.tamMax + er_multiplicador.tamMax, sizeof(char)*MAX_STR_PRINT);
    print_uf = (char**)calloc(somador.tamMax + multiplicador.tamMax, sizeof(char)*MAX_STR_PRINT);
    print_bf = (char**)calloc(load.tamMax + store.tamMax, sizeof(char)*MAX_STR_PRINT);
    print_esc = (char**)calloc(load.tamMax + store.tamMax + somador.tamMax + multiplicador.tamMax, sizeof(char)*MAX_STR_PRINT);
    print_ue = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    print_mem = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
}

void inicializaPrint(){
    num_emitidas = 0;
    num_print_er = 0;
    num_print_uf = 0;
    num_print_ue = 0;
    num_print_mem = 0;
    num_print_bf = 0;
    num_print_esc = 0;
}

void inserePrintEmitidas(Instrucao inst){
    print_emitidas[num_emitidas] = inst;
    num_emitidas++;
}

void inserePrintER(EstacaoReserva er, int posicao, char nome[]){
    print_er[num_print_er] = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    char *str = erToString(er);
    sprintf(print_er[num_print_er], "%s(%s%d)", str, nome, posicao);
    free(str);
    num_print_er++;
}

void inserePrintUF(UnidadeFuncional uf, int posicao, char nome[]){
    print_uf[num_print_uf] = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    char *str = ufToString(uf);
    sprintf(print_uf[num_print_uf], "%s(%s%d)", str, nome, posicao);
    free(str);
    num_print_uf++;
}

void inserePrintBF(Buffer buffer, int posicao, char nome[]){
    print_bf[num_print_bf] = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    char *str = bfToString(buffer);
    sprintf(print_bf[num_print_bf], "%s(%s%d)", str, nome, posicao);
    free(str);
    num_print_bf++;
}

void inserePrintESC(int valor, int reg, int estacao){
    print_esc[num_print_esc] = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    if (reg == -1){
        char *str = qiToString(estacao);
        sprintf(print_esc[num_print_esc], "%s <- %d", str, valor);
        free(str);
    }
    else{
        sprintf(print_esc[num_print_esc], "R%d <- %d", reg, valor);
    }
    num_print_esc++;
}

void inserePrintEscSD(int destino, int valor){
    print_esc[num_print_esc] = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    sprintf(print_esc[num_print_esc], "[%d] <- %d", destino, valor);
    num_print_esc++;
}

void inserePrintUE(UnidadeEndereco ue){ 
    char *str = ueToString(ue);
    sprintf(print_ue, "%s", str);
    free(str);
    num_print_ue++;
}

void inserePrintMEM(MemoriaExec mem_exec){ 
    char *str = memToString(mem_exec);
    sprintf(print_mem, "%s", str);
    free(str);
    num_print_mem++;
}

/* TOMASULO */ 

//busca instruções na memória e coloca na fila
bool busca(){
    Instrucao inst;
    int i;
    for (i = 0; i < qtd_busca_inst && !filaEstaCheia(fila); i++){
        inst = memoriaObterInst(pc);    //busca a proxima inst
        inst.id = id++;                 //id para dependencias
        switch (inst.opcode){
            case EXIT:
                return !filaEstaVazia(fila);
                break;
            case JUMP:
                pc += inst.dest;
                break;
            case NOP:
                pc++;
                break;
            case BEQ:
            case BNE:
            case BG:
            case BGE:
            case BL:
            case BLE:
                //QUANDO FOR JMP NAO PODE BUSCAR OUTRAS INSTRUÇÕES ENQUANTO NAO EXECUTAR O JMP
                filaInsere(fila, inst);
                pc++;
                break;
            default:
                filaInsere(fila, inst);
                pc++;
                break;
        }
    }
    return !filaEstaVazia(fila);
}

void verificaQI(int reg, int *q, int *v){
    if (registrador[reg].qi == -1){
        *v = registrador[reg].valor;
        *q = -1;
    }
    else {        
        *q = registrador[reg].qi;
        *v = registrador[reg].id;
    }
}

//QUANDO FOR JUMP EMITIR NOP
//retornar numero de emitidas, se for -1 nao emitiu nada(false)
bool emissao(){
    int i, posicao, pos_anterior_somador = 0, pos_anterior_mult = 0;
    int qj, qk, vj, vk, A;
    Instrucao inst;
    bool foiEmitida;

    for(i = 0; i < qtd_emissao && !filaEstaVazia(fila); i++){
        qj = -1;
        qk = -1;
        vj = 0;
        vk = 0;
        A = 0;
        foiEmitida = false;
        inst = filaPrimeiro(fila);
        switch (inst.opcode){
            case LD:
                if(!uEnderecoCheia()){
                    posicao = procuraBuffer(load);
                    filaRemove(fila, &inst);
                    uEnderecoInsere(&unidadeEndereco, inst.opcode, inst.id, inst.op1, inst.dest, qj);
                    registradorMudaQI(registrador, inst.dest, inst.id, 0, LOAD);
                    foiEmitida = true;
                }
                break;
            case SD:                    
                if(!uEnderecoCheia()){
                    filaRemove(fila, &inst);                    
                    verificaQI(inst.op1, &qj, &vj);                    
                    uEnderecoInsere(&unidadeEndereco, inst.opcode, inst.id, vj, inst.dest, qj);
                    foiEmitida = true;
                }
                break;
            case LI:
                posicao = procuraUF(somador, pos_anterior_somador);

                if (posicao == -1) //UF cheia
                    posicao = procuraEstacao(er_somador); //procura ER livre
                else
                    pos_anterior_somador = posicao + 1; //tem pos UF
                
                if (posicao > -1 && !er_somador.est_reserva[posicao].busy){ //Se houver posicao e nao tiver dep.                       
                    filaRemove(fila, &inst);
                    vj = inst.op1;
                    estacaoInsere(&er_somador, inst.id, inst.opcode, qj, qk, vj, vk, A, posicao); //Insere na ER
                    registradorMudaQI(registrador, inst.dest, inst.id, posicao, SOMADOR);
                    foiEmitida = true; //Marca que foi emitida
                }
                break;
            case BEQ:
            case BNE:
            case BG:
            case BGE:
            case BL:
            case BLE:
                posicao = procuraUF(somador, pos_anterior_somador);
                if (posicao == -1)
                    posicao = procuraEstacao(er_somador);
                else
                    pos_anterior_somador = posicao + 1;
                
                if (posicao > -1 && !er_somador.est_reserva[posicao].busy){
                    filaRemove(fila, &inst);
                    verificaQI(inst.op1, &qj, &vj);
                    verificaQI(inst.op2, &qk, &vk);
                    A = inst.dest;
                    estacaoInsere(&er_somador, inst.id, inst.opcode, qj, qk, vj, vk, A, posicao);
                    foiEmitida = true;
                }
                break;
            case ADD:                    
            case SUB:
                posicao = procuraUF(somador, pos_anterior_somador);
                if (posicao == -1)
                    posicao = procuraEstacao(er_somador);
                else
                    pos_anterior_somador = posicao + 1;

                if (posicao > -1 && !er_somador.est_reserva[posicao].busy){
                    filaRemove(fila, &inst);
                    verificaQI(inst.op1, &qj, &vj);
                    verificaQI(inst.op2, &qk, &vk);
                    estacaoInsere(&er_somador, inst.id, inst.opcode, qj, qk, vj, vk, A, posicao);
                    registradorMudaQI(registrador, inst.dest, inst.id, posicao, SOMADOR);
                    foiEmitida = true;
                }
                break;
            case ADDI:
            case SUBI:
                posicao = procuraUF(somador, pos_anterior_somador);
                if (posicao == -1)
                    posicao = procuraEstacao(er_somador);
                else
                    pos_anterior_somador = posicao + 1;

                if (posicao > -1 && !er_somador.est_reserva[posicao].busy){                        
                    filaRemove(fila, &inst);
                    verificaQI(inst.op1, &qj, &vj);
                    vk = inst.op2;
                    estacaoInsere(&er_somador, inst.id, inst.opcode, qj, qk, vj, vk, A, posicao);
                    registradorMudaQI(registrador, inst.dest, inst.id, posicao, SOMADOR);
                    foiEmitida = true;
                }
                break;
            case MULT:
            case DIV:
                posicao = procuraUF(multiplicador, pos_anterior_mult);
                if (posicao == -1)
                    posicao = procuraEstacao(er_multiplicador);
                else
                    pos_anterior_mult = posicao + 1;

                if (posicao > -1 && !er_multiplicador.est_reserva[posicao].busy){
                    filaRemove(fila, &inst);
                    verificaQI(inst.op1, &qj, &vj);
                    verificaQI(inst.op2, &qk, &vk);
                    estacaoInsere(&er_multiplicador, inst.id, inst.opcode, qj, qk, vj, vk, A, posicao);
                    registradorMudaQI(registrador, inst.dest, inst.id, posicao, MULTIPLICADOR);
                    foiEmitida = true;
                }
                break;
            case MULTI:
            case DIVI:
                posicao = procuraUF(multiplicador, pos_anterior_mult);
                if (posicao == -1)
                    posicao = procuraEstacao(er_multiplicador);
                else
                    pos_anterior_mult = posicao + 1;

                if (posicao > -1 && !er_multiplicador.est_reserva[posicao].busy){
                    filaRemove(fila, &inst);
                    verificaQI(inst.op1, &qj, &vj);
                    vk = inst.op2;
                    estacaoInsere(&er_multiplicador, inst.id, inst.opcode, qj, qk, vj, vk, A, posicao);
                    registradorMudaQI(registrador, inst.dest, inst.id, posicao, MULTIPLICADOR);
                    foiEmitida = true;
                }
                break;
            default:
                break;
        }
        if (foiEmitida)
            inserePrintEmitidas(inst);
    }

    return fila->tam > 0 || num_emitidas > 0;
}

bool escrita(){
    int i, reg;
    bool flag = false;
    int64_t valor;
    for (i = 0; i < barramento.tamMax; ++i){
        if (barramento.campo[i].id != -1){
            reg = procuraRegistrador(registrador, i);
            valor = barramento.campo[i].dado;
            if (reg != -1)
                registradorEscreve(registrador, reg, valor);
            
            inserePrintESC(valor, reg, i);
            flag = true;
        }
    }
    return flag;
}

bool pulso(){
    int i;

    /* ATUALIZA SOMADOR */
    for (i = 0; i < somador.tamMax; ++i) {
        if (somador.un_funcional[i].busy){            
            inserePrintUF(somador.un_funcional[i], i, "US");
            somador.un_funcional[i].ciclos--;
        }
    }

    /* ATUALIZA MULTIPLICADOR */
    for (i = 0; i < multiplicador.tamMax; ++i) {
        if (multiplicador.un_funcional[i].busy){            
            inserePrintUF(multiplicador.un_funcional[i], i, "UM");
            multiplicador.un_funcional[i].ciclos--;            
        }
    }

    /* ATUALIZA MEMÓRIA */
    if (mem_exec.busy){
        inserePrintMEM(mem_exec);
        mem_exec.ciclos--;
    }
    
    return somador.tam > 0 || multiplicador.tam > 0 || mem_exec.busy;
}

void verificaBarramento(EstacaoReserva *er){
    if (er->qj != -1){
        if (er->vj == barramento.campo[er->qj].id){
            er->vj = barramento.campo[er->qj].dado;
            er->qj = -1;
        }
    }
    if (er->qk != -1){
        if (er->vk == barramento.campo[er->qk].id){
            er->vk = barramento.campo[er->qk].dado;
            er->qk = -1;
        }
    }
}

bool execucao(){
    int i;
    int posicao;
    bool flag_sd = false;

    /* EXECUÇÃO SOMADOR */
    int vj, vk;
    for (i = 0; i < somador.tamMax; ++i) {
        if (somador.un_funcional[i].busy){
            if (somador.un_funcional[i].ciclos == 0){ //quando chegar no ciclo 0, realiza a operacao
                vj = somador.un_funcional[i].vj;
                vk = somador.un_funcional[i].vk;
                switch (somador.un_funcional[i].opcode){
                    case BEQ:
                        break;
                    case BNE:
                        break;
                    case BG: 
                        break;
                    case BGE:
                        break;
                    case BL: 
                        break;
                    case BLE:
                        break;
                    case LI:
                    case ADD:
                    case ADDI:
                        barramentoInsere(&barramento, vj + vk, somador.un_funcional[i].id, i, SOMADOR);
                        break;
                    case SUB:
                    case SUBI:
                        barramentoInsere(&barramento, vj - vk, somador.un_funcional[i].id, i, SOMADOR);
                        break;
                    default:
                        break;
                }
                uFuncionalRemove(&somador, i); 
            }            
        }
    }

    /* EXECUÇÃO MULTIPLICADOR */
    for (i = 0; i < multiplicador.tamMax; ++i) {
        if (multiplicador.un_funcional[i].busy){
            if (multiplicador.un_funcional[i].ciclos == 0){
                vj = multiplicador.un_funcional[i].vj;
                vk = multiplicador.un_funcional[i].vk;
                switch (multiplicador.un_funcional[i].opcode){
                    case MULT: 
                    case MULTI:
                        barramentoInsere(&barramento, vj * vk, multiplicador.un_funcional[i].id, i, MULTIPLICADOR);
                        break;
                    case DIV:
                    case DIVI:
                        barramentoInsere(&barramento, vj / vk, multiplicador.un_funcional[i].id, i, MULTIPLICADOR);
                        break;
                    default:
                        break;
                }
                uFuncionalRemove(&multiplicador, i);
            }            
        }
    }

    /* EXECUÇÃO MEMORIA */
    if (mem_exec.busy){
        if (mem_exec.ciclos == 0){
            switch (mem_exec.opcode){
                case LD:                
                    barramentoInsere(&barramento, memoriaObterDado(mem_exec.origem), mem_exec.id, 0, LOAD);
                    break;
                case SD:
                    flag_sd = true;                    
                    memoriaInsereDado(mem_exec.origem, mem_exec.destino);
                    inserePrintEscSD(mem_exec.destino, mem_exec.origem);
                    break;
                default:
                    break;
            }
            memoriaExecRemove(&mem_exec);
        }
    }

    /* Verifica ER Somador */
    for (i = 0; i < er_somador.tamMax; ++i) {
        if (er_somador.est_reserva[i].busy){
            verificaBarramento(&er_somador.est_reserva[i]);
            if (!somador.un_funcional[i].busy && er_somador.est_reserva[i].qj == -1 && er_somador.est_reserva[i].qk == -1){                
                switch (er_somador.est_reserva[i].opcode){
                     case LI:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, LI, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_li);
                         break;
                     case BEQ:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, BEQ, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_beq);
                         break;
                     case BNE:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, BNE, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bne);
                         break;
                     case BG:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, BG, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bg);
                         break;
                     case BGE:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, BGE, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bge);
                         break;
                     case BL:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, BL, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bl);
                         break;
                     case BLE:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, BLE, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_ble);
                         break;
                     case ADD:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, ADD, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_add);
                         break;
                     case ADDI:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, ADDI, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_addi);
                         break;
                     case SUB:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, SUB, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_sub);
                         break;
                     case SUBI:
                         uFuncionalInsere(&somador, i, er_somador.est_reserva[i].id, SUBI, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_subi);
                         break;
                     default:
                         break;
                }
                estacaoRemove(&er_somador, i);
            }
            else {
                inserePrintER(er_somador.est_reserva[i], i, "ES");
            }
        }
    }

    /* Verifica ER Mutiplicador */
    for (i = 0; i < er_multiplicador.tamMax; ++i) {
        if (er_multiplicador.est_reserva[i].busy){
            verificaBarramento(&er_multiplicador.est_reserva[i]);
            if (!multiplicador.un_funcional[i].busy && er_multiplicador.est_reserva[i].qj == -1 && er_multiplicador.est_reserva[i].qk == -1){
                switch (er_multiplicador.est_reserva[i].opcode){
                    case MULT:
                        uFuncionalInsere(&multiplicador, i, er_multiplicador.est_reserva[i].id, MULT, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_mult);
                        break;
                    case MULTI:
                        uFuncionalInsere(&multiplicador, i, er_multiplicador.est_reserva[i].id, MULTI, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_multi);
                        break;
                    case DIV:
                        uFuncionalInsere(&multiplicador, i, er_multiplicador.est_reserva[i].id, DIV, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_div);
                        break;
                    case DIVI:
                        uFuncionalInsere(&multiplicador, i, er_multiplicador.est_reserva[i].id, DIVI, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_divi);
                        break;
                    default:
                        break;
                }
                estacaoRemove(&er_multiplicador, i);
            }  
            else {
                inserePrintER(er_multiplicador.est_reserva[i], i, "EM");
            }          
        }
    }
    
    if (load.buffer[0].busy && store.buffer[0].busy && !mem_exec.busy){
        if (load.buffer[0].id < store.buffer[0].id){
            memoriaExecInsere(&mem_exec, load.buffer[0].id, load.buffer[0].opcode, load.buffer[0].origem, load.buffer[0].destino, ciclo_ld);
            bufferRemove(&load, 0);
        }
        else {
            if (store.buffer[0].q != -1){
                if (store.buffer[0].origem == barramento.campo[store.buffer[0].q].id){
                    store.buffer[0].origem = barramento.campo[store.buffer[0].q].dado;
                    store.buffer[0].q = -1;
                }
            }
            else {
                memoriaExecInsere(&mem_exec, store.buffer[0].id, store.buffer[0].opcode, store.buffer[0].origem, store.buffer[0].destino, ciclo_sd);                
                bufferRemove(&store, 0);
            }
        }
    }
    else if (load.buffer[0].busy && !mem_exec.busy){
        memoriaExecInsere(&mem_exec, load.buffer[0].id, load.buffer[0].opcode, load.buffer[0].origem, load.buffer[0].destino, ciclo_ld);
        bufferRemove(&load, 0);
    }
    else if (store.buffer[0].busy && !mem_exec.busy) {
        if (store.buffer[0].q != -1){
            if (store.buffer[0].origem == barramento.campo[store.buffer[0].q].id){
                store.buffer[0].origem = barramento.campo[store.buffer[0].q].dado;
                store.buffer[0].q = -1;
            }
        }
        else {
            memoriaExecInsere(&mem_exec, store.buffer[0].id, store.buffer[0].opcode, store.buffer[0].origem, store.buffer[0].destino, ciclo_sd);                
            bufferRemove(&store, 0);
        }
    }

    /* ATUALIZA LOAD */
    for (i = 0; i < load.tamMax; ++i) {
        if(load.buffer[i].busy){      
            inserePrintBF(load.buffer[i], i, "BL");
        }
    }
    
    /* ATUALIZA STORE */
    for (i = 0; i < store.tamMax; ++i) {
        if(store.buffer[i].busy){            
            inserePrintBF(store.buffer[i], i, "BS");
        }
    }
    
    /* ATUALIZA UN. ENDERECO */
    if(unidadeEndereco.busy){
        inserePrintUE(unidadeEndereco);
        switch(unidadeEndereco.opcode){
            case LD:
                posicao = procuraBuffer(load);
                if(posicao > -1){
                    bufferInsere(&load, unidadeEndereco.opcode, unidadeEndereco.id, unidadeEndereco.origem, unidadeEndereco.destino, unidadeEndereco.q);
                    uEnderecoRemove(&unidadeEndereco);
                }
                break;
            case SD:
                posicao = procuraBuffer(store);
                if(posicao > -1){
                    if (unidadeEndereco.q != -1){
                        if (unidadeEndereco.origem == barramento.campo[unidadeEndereco.q].id){
                            unidadeEndereco.origem = barramento.campo[unidadeEndereco.q].dado;
                            unidadeEndereco.q = -1;
                        }
                    }
                    bufferInsere(&store, unidadeEndereco.opcode, unidadeEndereco.id, unidadeEndereco.origem, unidadeEndereco.destino, unidadeEndereco.q);
                    uEnderecoRemove(&unidadeEndereco);
                }
                break;
            default:
                break;
        }
    }

    return er_somador.tam > 0 || er_multiplicador.tam > 0 || load.tam > 0 || store.tam > 0 || flag_sd;
}

void iniciarTomasulo(){
    pc = 0;
    cont_ciclos = 0;
    id = 0;
    bool flag_busca, flag_emissao, flag_pulso, flag_exec, flag_esc;
    criaVetorPrint();
    
    while (true) {
        inicializaPrint();
        barramentoLimpar(&barramento);

        flag_exec = execucao();
        flag_pulso = pulso();
        flag_esc = escrita();
        flag_emissao = emissao();
        flag_busca = busca();

        if (!flag_busca && !flag_emissao && !flag_pulso && !flag_exec && !flag_esc)
            break;

        cont_ciclos++;
        printCiclo();
    }
}