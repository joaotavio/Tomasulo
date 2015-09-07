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
int ciclo_lui;

int cont_ciclos;
int id;

/* REGISTRADORES ESPECÍFICOS */
int pc;
bool flag_jmp;
bool flag_memoria_busy;

/* VARIÁVEIS PARA PRINT */

Instrucao *print_emitidas;
int num_emitidas;
char **print_er;
int num_print_er;
char **print_uf;
int num_print_uf;
char *print_ue;
int num_print_ue;
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

//fazer printMemoria();

void printCiclo(){
    int i, num;
    char str[MAX_STR_PRINT];
    num = MAX(num_emitidas, MAX(num_print_er, MAX(num_print_uf, MAX(num_print_bf, MAX(num_print_ue, num_print_esc)))));

    printf("\nCICLO: %d\n", cont_ciclos);
    printf("%-20s | %-20s | %-20s | %-20s | %-20s | %-20s |\n", "EMISSAO", "UNIDADE DE ENDERECO", "BUFFER LOAD/STORE", "ESTACAO DE RESERVA", "UNIDADE FUNCIONAL", "ESCRITA");
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|----------------------|\n");
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


        /* ESCRITA */
        if (num_print_esc <= i)
            strcpy(str, "---");
        else
            strcpy(str, print_esc[i]);
        printf("%-20s | ", str);


        printf("\n");        
    }
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|----------------------|\n\n");
    printRegistrador();
}

void criaVetorPrint(){
    print_emitidas = (Instrucao*)calloc(100, sizeof(Instrucao));
    print_er = (char**)calloc(er_somador.tam + er_multiplicador.tam, sizeof(char)*MAX_STR_PRINT);
    print_uf = (char**)calloc(somador.tam + multiplicador.tam, sizeof(char)*MAX_STR_PRINT);
    print_bf = (char**)calloc(load.tam + store.tam, sizeof(char)*MAX_STR_PRINT);
    print_esc = (char**)calloc(load.tam + store.tam + somador.tam + multiplicador.tam, sizeof(char)*MAX_STR_PRINT);
    print_ue = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
}

void inicializaPrint(){
    num_emitidas = 0;
    num_print_er = 0;
    num_print_uf = 0;
    num_print_ue = 0;
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

void inserePrintESC(int posicao, int valor, char str[]){
    print_esc[num_print_esc] = (char*)malloc(sizeof(char)*MAX_STR_PRINT);
    sprintf(print_esc[num_print_esc], "%s%d <- %d", str, posicao, valor);
    num_print_esc++;
}

void inserePrintUE(UnidadeEndereco ue){ 
    char *str = ueToString(ue);
    sprintf(print_ue, "%s", str);
    free(str);
    num_print_ue++;
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
                    filaRemove(fila, &inst);                        
                    uEnderecoInsere(&unidadeEndereco, inst.opcode, inst.op1, inst.dest);
                    foiEmitida = true;
                }
                break;
            case SD:                    
                if(!uEnderecoCheia()){
                    filaRemove(fila, &inst);
                    uEnderecoInsere(&unidadeEndereco, inst.opcode, inst.op1, inst.dest);
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
    int i;
    bool flag = false;
    for (i = 0; i < barramento.tamMax; ++i){
        if (barramento.campo[i].id != -1){

            flag = true;
        }
    }
    return false;
}

void escreve(int64_t valor, int id, int estacao, TipoComponente tipo, char str[]){
    barramentoInsere(&barramento, valor, id, estacao, tipo);
    int reg = procuraRegistrador(registrador, estacao, tipo);
    if (reg == -1){
        inserePrintESC(estacao, valor, str);
    }
    else {
        inserePrintESC(reg, valor, "R");
        registradorEscreve(registrador, reg, valor);
    }
}

void verificaBarramento(EstacaoReserva *er, TipoComponente tipo){
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

bool pulso(){
    int i;
    int posicao;
    
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
                        escreve(vj + vk, somador.un_funcional[i].id, i, SOMADOR, "ES");
                        break;
                    case SUB:
                    case SUBI:
                        escreve(vj - vk, somador.un_funcional[i].id, i, SOMADOR, "ES");
                        break;
                    default:
                        break;
                }
                uFuncionalRemove(&somador, i); 
            }            
        }
    }

    /* EXECUCAO MULTIPLICADOR */
    for (i = 0; i < multiplicador.tamMax; ++i) {
        if (multiplicador.un_funcional[i].busy){
            if (multiplicador.un_funcional[i].ciclos == 0){
                vj = multiplicador.un_funcional[i].vj;
                vk = multiplicador.un_funcional[i].vk;
                switch (multiplicador.un_funcional[i].opcode){
                    case MULT: 
                    case MULTI:
                        escreve(vj * vk, multiplicador.un_funcional[i].id, i, MULTIPLICADOR, "EM");
                        break;
                    case DIV:
                    case DIVI:
                        escreve(vj / vk, multiplicador.un_funcional[i].id, i, MULTIPLICADOR, "EM");
                        break;
                    default:
                        break;
                }
                uFuncionalRemove(&multiplicador, i);
            }            
        }
    }

    /* Verifica ER Somador */
    for (i = 0; i < er_somador.tamMax; ++i) {
        if (er_somador.est_reserva[i].busy){
            verificaBarramento(&er_somador.est_reserva[i], SOMADOR);
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
            verificaBarramento(&er_multiplicador.est_reserva[i], MULTIPLICADOR);
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

    
    
    /*ATUALIZA LOAD/STORE */
    for (i = 0; i < load.tamMax; ++i) {
        if(load.buffer[i].busy){
            //if(!memoria.busy){
                //memoriaInsere();
                bufferRemove(&load, i); 
                //printf("\nOrigem: %d\n", load.buffer[i].origem);
                //printf("\nDestino: %d\n", load.buffer[i].destino);
            //} else {
                //inserePrintBF(load.buffer[i], i, "BL");
            //}
        }
    }
    
    for (i = 0; i < store.tamMax; ++i) {
        if(store.buffer[i].busy){
            //if(!memoria.busy){
                //memoriaInsere();
                bufferRemove(&store, i);
                //printf("\nOrigem: %d\n", store.buffer[i].origem);
                //printf("\nDestino: %d\n", store.buffer[i].destino);               
            //} else {
                //inserePrintBF(load.buffer[i], i, "BS");
            //}
        }
    }
    
    /* ATUALIZA UN. ENDERECO */
    if(unidadeEndereco.busy){
        inserePrintUE(unidadeEndereco);
        switch(unidadeEndereco.opcode){
            case LD:
                posicao = procuraBuffer(load);
                if(posicao > -1){
                    bufferInsere(&load, unidadeEndereco.opcode, unidadeEndereco.origem, unidadeEndereco.destino);
                    uEnderecoRemove(&unidadeEndereco);
                }
                break;
            case SD:
                posicao = procuraBuffer(store);
                if(posicao > -1){
                    bufferInsere(&load, unidadeEndereco.opcode, unidadeEndereco.origem, unidadeEndereco.destino);
                    uEnderecoRemove(&unidadeEndereco);
                }
                break;
            default:
                break;
        }
    }
    
    
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

    return er_somador.tam > 0 || somador.tam > 0 || er_multiplicador.tam > 0 || multiplicador.tam > 0;;
}


bool execucao(){
    return false;
}

void iniciarTomasulo(){
    pc = 0;
    cont_ciclos = 0;
    id = 0;
    bool flag_busca, flag_emissao, flag_pulso, flag_exec, flag_esc;
    criaVetorPrint();

    while (true) {
        inicializaPrint();
        
        flag_pulso = pulso();
        flag_esc = escrita();
        flag_exec = execucao();
        flag_emissao = emissao();
        flag_busca = busca();

        if (!flag_busca && !flag_emissao && !flag_pulso && !flag_exec && !flag_esc)
            break;

        cont_ciclos++;
        printCiclo();
        barramentoLimpar(&barramento);
    }
    /*int i;
    printf("----------------------------------------------------------\n");
    for(i = 0; i < somador.tamMax; i++){
        printf("Opcode %d\n", er_somador.est_reserva[i].opcode);
        printf("QJ: %d\n", er_somador.est_reserva[i].qj);
        printf("QK: %d\n", er_somador.est_reserva[i].qk);
        printf("Busy: %d\n\n", er_somador.est_reserva[i].busy);
        printf("TAM: %d\n", er_somador.tam);
    }
    printf("\n\nSOMADOR\n");
    for(i = 0; i < somador.tamMax; i++){
        printf("Opcode %d\n", somador.un_funcional[i].opcode);
        printf("VJ: %d\n", somador.un_funcional[i].vj);
        printf("VK: %d\n", somador.un_funcional[i].vk);
        printf("Busy: %d\n\n", somador.un_funcional[i].busy);
        printf("TAM: %d\n", somador.tam);
    }
    printf("----------------------------------------------------------\n");*/
}