#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tomasulo.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*** VARIÁVEIS GLOBAIS ***/

int qtd_busca_inst;
int qtd_emissao;
int qtd_portas_reg;

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
int pc;

/* VARIÁVEIS PARA PRINT */

Instrucao print_emitidas[100]; //DEPOIS MUDAR ISSO PARA MALLOC - TALVEZ PODE USAR ISSO PRA DEPENDENCIA
int num_emitidas;
char *print_er[100];
int num_print_er;
char *print_uf[100];
int num_print_uf;

/* FUNÇÕES PARA PRINT */

void printCiclo(){
    int i, num;
    char *str = (char*)malloc(sizeof(char));
    num = MAX(num_emitidas, MAX(num_print_er, num_print_uf));

    printf("CICLO: %d\n", cont_ciclos);
    printf("%-20s | %-20s | %-20s | %-20s | %-20s |\n", "EMISSAO", "UNIDADE DE ENDERECO", "ESTACAO DE RESERVA", "UNIDADE FUNCIONAL", "ESCRITA");
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|\n");
    for (i = 0; i < num; ++i) {
        /* EMITIDAS */
        if (num_emitidas <= i)
            strcpy(str, "---");
        else
            str = instToString(print_emitidas[i]);
        printf("%-20s | ", str);
        free(str);

        /* UNIDADE DE ENDEREÇO */
        //if (num_print_ue <= i)
            strcpy(str, "---");
        //else
            //str = instToString(print_emitidas[i]);
        printf("%-20s | ", str);
        free(str);

        /* ESTAÇÃO DE RESERVA */
        if (num_print_er <= i)
            strcpy(str, "---");
        else
            str = print_er[i];
        printf("%-20s | ", str);
        free(str);

        /* UNIDADE FUNCIONAL */
        if (num_print_uf <= i)
            strcpy(str, "---");
        else
            str = print_uf[i];
        printf("%-20s | ", str);
        free(str);


        /* ESCRITA */
        //if (num_emitidas <= i)
            strcpy(str, "---");
        //else
            //str = instToString(print_emitidas[i]);
        printf("%-20s | ", str);
        free(str);


        printf("\n");        
    }
    printf("---------------------|----------------------|----------------------|----------------------|----------------------|\n\n");
}

void inicializaPrint(){
    num_emitidas = 0;
    num_print_er = 0;
    num_print_uf = 0;
}

void inserePrintEmitidas(Instrucao inst){
    print_emitidas[num_emitidas] = inst;
    num_emitidas++;
}

void inserePrintER(EstacaoReserva er, int posicao, char nome[]){
    print_er[num_print_er] = (char*)malloc(sizeof(char));
    char *str = erToString(er);
    sprintf(print_er[num_print_er], "%s(%s%d)", str, nome, posicao);
    free(str);
    num_print_er++;
}

void inserePrintUF(UnidadeFuncional uf, int posicao, char nome[]){
    print_uf[num_print_uf] = (char*)malloc(sizeof(char));
    char *str = ufToString(uf);
    sprintf(print_uf[num_print_uf], "%s(%s%d)", str, nome, posicao);
    free(str);
    num_print_uf++;
}

void printRegistrador(){

}

/* TOMASULO */ 

//busca instruções na memória e coloca na janela
bool busca(){
    Instrucao inst;
    int i;
    for (i = 0; i < qtd_busca_inst && !janelaCheia(janela); i++){
        inst = memoriaObterInst(pc);
        switch (inst.opcode){
            case EXIT:
                return !janelaVazia(janela);
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
                janelaInsere(inst);
                pc++;
                break;
            default:
                janelaInsere(inst);
                pc++;
                break;
        }
    }
    return !janelaVazia(janela);
}

//procurar unidade funcional livre e colocar na ER associada, se nenhuma tiver livre procura uma estacao de reserva
//retornar numero de emitidas, se for -1 nao emitiu nada(false)
bool emissao(){
    int i, j, posicao, pos_anterior_somador = 0, pos_anterior_mult = 0/*, pos_anterior_load = 0, pos_anterior_store = 0*/;
    Instrucao inst;
    bool foiEmitida;

    j = 0;
    for(i = 0; i < qtd_emissao && !janelaVazia(janela); i++){
        foiEmitida = false;
        while (!foiEmitida && j < janela.tam) {
            inst = janela.inst[j];
            switch (inst.opcode){
                case LD:
					//posicao = procuraMemoria(só ver a memoria ta livre?);
					//if(posicao == -1){
						posicao = procuraBuffer(load);
					//} else{
						//pos_anterior_load = posicao + 1;
					//}
					
					if(posicao > -1){
						janelaRemove(j);
						bufferInsere(&load, inst.op1, inst.dest);
						foiEmitida = true;
					}
                    break;
                case SD:                    
                    janelaRemove(j);
                    foiEmitida = true;
                    break;
                case LI: //ok
                    posicao = procuraUF(somador, pos_anterior_somador);

                    if (posicao == -1)
                        posicao = procuraEstacao(er_somador);
                    else
                        pos_anterior_somador = posicao + 1;
                    
                    if (posicao > -1){                        
                        janelaRemove(j);
                        estacaoInsere(&er_somador, inst.opcode, -1, -1, inst.op1, 0, posicao);
                        foiEmitida = true;
                    }                  
                    break;
                case BEQ: //ok
                case BNE: //ok
                case BG: //ok
                case BGE: //ok
                case BL: //ok
                case BLE: //ok
                    posicao = procuraUF(somador, pos_anterior_somador);
                    if (posicao == -1)
                        posicao = procuraEstacao(er_somador);
                    else
                        pos_anterior_somador = posicao + 1;
                    
                    if (posicao > -1){
                        janelaRemove(j);
                        estacaoInsere(&er_somador, inst.opcode, inst.op1, inst.op2, 0, 0, posicao);
                        foiEmitida = true;
                    }
                    break;
                case ADD: //ok             
                case SUB:  //ok
                    posicao = procuraUF(somador, pos_anterior_somador);
                    if (posicao == -1)
                        posicao = procuraEstacao(er_somador);
                    else
                        pos_anterior_somador = posicao + 1;

                    if (posicao > -1){
                        janelaRemove(j);
                        estacaoInsere(&er_somador, inst.opcode, inst.op1, inst.op2, 0, 0, posicao);
                        foiEmitida = true;
                    }                   
                    break;
                case ADDI:  //ok
                case SUBI: //ok
                    posicao = procuraUF(somador, pos_anterior_somador);
                    if (posicao == -1)
                        posicao = procuraEstacao(er_somador);
                    else
                        pos_anterior_somador = posicao + 1;

                    if (posicao > -1){
                        janelaRemove(j);                        
                        estacaoInsere(&er_somador, inst.opcode, inst.op1, -1, 0, inst.op2, posicao);
                        foiEmitida = true;
                    }
                    break;
                case MULT:  //ok                   
                case DIV: //ok
                    posicao = procuraUF(multiplicador, pos_anterior_mult);
                    if (posicao == -1)
                        posicao = procuraEstacao(er_multiplicador);
                    else
                        pos_anterior_mult = posicao +1 ;

                    if (posicao > -1){
                        janelaRemove(j);
                        estacaoInsere(&er_multiplicador, inst.opcode, inst.op1, inst.op2, 0, 0, posicao);
                        foiEmitida = true;
                    }
                    break;
                case MULTI:   //ok                  
                case DIVI: //ok
                    posicao = procuraUF(multiplicador, pos_anterior_mult);
                    if (posicao == -1)
                        posicao = procuraEstacao(er_multiplicador);
                    else
                        pos_anterior_mult = posicao +1 ;

                    if (posicao > -1){
                        janelaRemove(j);                        
                        estacaoInsere(&er_multiplicador, inst.opcode, inst.op1, -1, 0, inst.op2, posicao);
                        foiEmitida = true;
                    }
                    break;
                default:
                    break;
            }
            if (!foiEmitida)
                j++;
        }
        if (foiEmitida)
            inserePrintEmitidas(inst);
    }

    return janela.tam > 0 || num_emitidas > 0; //return janela.tam==0
}

//VERIFICAR TODOS OS BUFFERS, UF, ER ETC
bool pulso(){
    int i;

    /* EXECUÇÃO SOMADOR */
    for (i = 0; i < somador.tamMax; ++i) {
        if (somador.un_funcional[i].busy){
            if (somador.un_funcional[i].ciclos == 0){
                switch (somador.un_funcional[i].opcode){
                    case LI: 
                        break;
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
                    case ADD:
                        break;
                    case ADDI:
                        break;
                    case SUB:
                        break;
                    case SUBI:
                        break;
                    default:
                        break;
                }
                //chamar função escreve()
                uFuncionalRemove(&somador, i);
            }            
        }
    }

    for (i = 0; i < multiplicador.tamMax; ++i) {
        if (multiplicador.un_funcional[i].busy){
            if (multiplicador.un_funcional[i].ciclos == 0){
                switch (multiplicador.un_funcional[i].opcode){
                    case MULT: 
                        break;
                    case MULTI:
                        break;
                    case DIV:
                        break;
                    case DIVI:
                        break;
                    default:
                        break;
                }
                //chamar função escreve()
                uFuncionalRemove(&multiplicador, i);
            }            
        }
    }

    /* Verifica ER Somador */
    for (i = 0; i < er_somador.tamMax; ++i) {
        if (er_somador.est_reserva[i].busy){
            inserePrintER(er_somador.est_reserva[i], i, "ES");
            if (!somador.un_funcional[i].busy){
                switch (er_somador.est_reserva[i].opcode){
                    case LI:
                        uFuncionalInsere(&somador, i, LI, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_li);
                        break;
                    case BEQ:
                        uFuncionalInsere(&somador, i, BEQ, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_beq);
                        break;
                    case BNE:
                        uFuncionalInsere(&somador, i, BNE, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bne);
                        break;
                    case BG:
                        uFuncionalInsere(&somador, i, BG, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bg);
                        break;
                    case BGE:
                        uFuncionalInsere(&somador, i, BGE, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bge);
                        break;
                    case BL:
                        uFuncionalInsere(&somador, i, BL, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_bl);
                        break;
                    case BLE:
                        uFuncionalInsere(&somador, i, BLE, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_ble);
                        break;
                    case ADD:
                        uFuncionalInsere(&somador, i, ADD, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_add);
                        break;
                    case ADDI:
                        uFuncionalInsere(&somador, i, ADDI, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_addi);
                        break;
                    case SUB:
                        uFuncionalInsere(&somador, i, SUB, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_sub);
                        break;
                    case SUBI:
                        uFuncionalInsere(&somador, i, SUBI, er_somador.est_reserva[i].vj, er_somador.est_reserva[i].vk, ciclo_subi);
                        break;
                    default:
                        break;
                }
                estacaoRemove(&er_somador, i);
            }            
        }
    }

    /* Verifica ER Mutiplicador */
    for (i = 0; i < er_multiplicador.tamMax; ++i) {
        if (er_multiplicador.est_reserva[i].busy){
            inserePrintER(er_multiplicador.est_reserva[i], i, "EM");
            if (!multiplicador.un_funcional[i].busy){
                switch (er_multiplicador.est_reserva[i].opcode){
                    case MULT:
                        uFuncionalInsere(&multiplicador, i, MULT, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_mult);                        
                        break;
                    case MULTI:
                        uFuncionalInsere(&multiplicador, i, MULTI, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_multi);
                        break;
                    case DIV:
                        uFuncionalInsere(&multiplicador, i, DIV, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_div);
                        break;
                    case DIVI:
                        uFuncionalInsere(&multiplicador, i, DIVI, er_multiplicador.est_reserva[i].vj, er_multiplicador.est_reserva[i].vk, ciclo_divi);
                        break;
                    default:
                        break;
                }
                estacaoRemove(&er_multiplicador, i);
            }            
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

void iniciarTomasulo(){
    pc = 0;
    cont_ciclos = 0;
    bool flag_busca, flag_emissao, flag_pulso;

    //loop principal
    while (true) {
        inicializaPrint();
        flag_pulso = pulso();
        flag_emissao = emissao();
        flag_busca = busca();

        if (!flag_busca && !flag_emissao && !flag_pulso)
            break;
        cont_ciclos++;
		int i;
		for(i = 0; i < load.tamMax; i++){
			printf("\n\nDestino: %d", load.buffer[i].destino);
			printf("\nOrigem: %d", load.buffer[i].origem);
			printf("\nBusy: %d\n", load.buffer[i].busy);
		}
        printCiclo();
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