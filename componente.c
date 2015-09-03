#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "componente.h"

Janela janela;
Registrador registrador[NUM_REGISTRADOR];
UnidadeEndereco unidadeEndereco;
ComponenteER er_somador;
ComponenteER er_multiplicador;
ComponenteUF somador;
ComponenteUF multiplicador;
ComponenteBuffer load;
ComponenteBuffer store;


/* JANELA */
void inicializaJanela(){
    janela.inst = (Instrucao*)calloc(janela.tamMax, sizeof(Instrucao));
    janela.tam = 0;
}

void janelaInsere(Instrucao inst){
    janela.inst[janela.tam] = inst;
    janela.tam++;
}

Instrucao janelaRemove(int posicao){
    if (janelaVazia(janela))
        return (Instrucao){0};
    Instrucao retorno = janela.inst[posicao];
	int i;
	
	for(i = posicao; i < (janela.tam - 1); i++){
		janela.inst[i] = janela.inst[i+1];
	}
	
	janela.tam--;
	
    return retorno;
}

bool janelaVazia(Janela janela){
    return janela.tam == 0;
}

bool janelaCheia(Janela janela){
    return janela.tam == janela.tamMax;
}

void mostraJanela(){
    int i;
    for (i = 0; i < janela.tam; ++i) {
        printInstrucao(janela.inst[i]);
    }
}

/* ESTAÇÃO DE RESERVA */
void inicializaER(ComponenteER *er, ComponenteUF *uf){
    er->est_reserva = (EstacaoReserva*)calloc(er->tamMax, sizeof(EstacaoReserva));
    uf->un_funcional = (UnidadeFuncional*)calloc(er->tamMax, sizeof(UnidadeFuncional));
    er->tam = 0;
    uf->tam = 0;
    uf->tamMax = er->tamMax;
}

bool estReservaCheia(ComponenteER er){
    return er.tam == er.tamMax;
}

void estacaoInsere(ComponenteER *er, Operacoes opcode, int qj, int qk, int vj, int vk, int posicao){
	er->est_reserva[posicao].opcode = opcode;
	er->est_reserva[posicao].qj = qj;
	er->est_reserva[posicao].qk = qk;
    er->est_reserva[posicao].vj = vj;
    er->est_reserva[posicao].vk = vk;
    er->est_reserva[posicao].busy = true;
    er->est_reserva[posicao].A = 0;
    er->tam++;
}

void estacaoRemove(ComponenteER *er, int posicao){
    er->est_reserva[posicao].opcode = NOP;
    er->est_reserva[posicao].qj = 0;
    er->est_reserva[posicao].qk = 0;
    er->est_reserva[posicao].busy = false;
    er->est_reserva[posicao].vj = 0;
    er->est_reserva[posicao].vk = 0;
    er->est_reserva[posicao].A = 0;
    er->tam--;
}

int procuraEstacao(ComponenteER er){
	int i;
	for(i = 0; i < er.tamMax; i++){
		if(er.est_reserva[i].busy == false){
			return i;
		}
	}
	
	return -1;
}

char* erToString(EstacaoReserva er){
    char *str = (char*)malloc(sizeof(char));
    char *strOpcode = (char*)malloc(sizeof(char));
    switch(er.opcode){
        case ADD:
            strcpy(strOpcode, "add");
            break;
        case SUB:
            strcpy(strOpcode, "sub");
            break;
        case MULT:
            strcpy(strOpcode, "mult");
            break;
        case DIV:
            strcpy(strOpcode, "div");
            break;
        case ADDI:
            strcpy(strOpcode, "addi");
            break;
        case SUBI:
            strcpy(strOpcode, "subi");
            break;
        case MULTI:
            strcpy(strOpcode, "multi");
            break;
        case DIVI:
            strcpy(strOpcode, "divi");
            break;        
        case LI:
            strcpy(strOpcode, "li");
            break;        
        case BEQ:
            strcpy(strOpcode, "beq");
            break;
        case BNE:
            strcpy(strOpcode, "bne");
            break;
        case BG:
            strcpy(strOpcode, "bg");
            break;
        case BGE:
            strcpy(strOpcode, "bge");
            break;
        case BL:
            strcpy(strOpcode, "bl");
            break;
        case BLE:
            strcpy(strOpcode, "ble");
            break;
        default:
            break;
    }
    if (er.qj < 0 && er.qk < 0)
        sprintf(str, "%s %d, %d", strOpcode, er.vj, er.vk);
    else if (er.qj < 0 && er.qk >= 0)
        sprintf(str, "%s %d, Q%d", strOpcode, er.vj, er.qk);
    else if (er.qj >= 0 && er.qk < 0)
        sprintf(str, "%s Q%d, %d", strOpcode, er.qj, er.vk);
    else
        sprintf(str, "%s Q%d, Q%d", strOpcode, er.qj, er.qk);

    free(strOpcode);
    return str;
}

/*void mostraEstacao(ComponenteBuffer *er, int tamanho){
    int i;
    for(i = 0; i < tamanho; i++){
        printf("Vk: %d\n", er[0].vk);
    }
}*/


/* UNIDADE FUNCIONAL */
bool unFuncionalCheia(ComponenteUF uf){
    return uf.tam == uf.tamMax;
}

void uFuncionalInsere(ComponenteUF *uf, int posicao, Operacoes opcode, int vj, int vk, int ciclos){
    uf->un_funcional[posicao].opcode = opcode;
    uf->un_funcional[posicao].vj = vj;
    uf->un_funcional[posicao].vk = vk;
    uf->un_funcional[posicao].busy = true;
    uf->un_funcional[posicao].ciclos = ciclos;
    uf->tam++;
}

void uFuncionalRemove(ComponenteUF *uf, int posicao){
    uf->un_funcional[posicao].opcode = NOP;
    uf->un_funcional[posicao].busy = false;
    uf->un_funcional[posicao].vj = 0;
    uf->un_funcional[posicao].vk = 0;
    uf->un_funcional[posicao].ciclos = 0;
    uf->tam--;
}

int procuraUF(ComponenteUF uf, int posicao){
    int i;
    for(i = posicao; i < uf.tamMax; i++){
        if(uf.un_funcional[i].busy == false){
            return i;
        }
    }
    
    return -1;
}

char* ufToString(UnidadeFuncional uf){
    char *str = (char*)malloc(sizeof(char));
    char *strOpcode = (char*)malloc(sizeof(char));
    switch(uf.opcode){
        case ADD:
            strcpy(strOpcode, "add");
            break;
        case SUB:
            strcpy(strOpcode, "sub");
            break;
        case MULT:
            strcpy(strOpcode, "mult");
            break;
        case DIV:
            strcpy(strOpcode, "div");
            break;
        case ADDI:
            strcpy(strOpcode, "addi");
            break;
        case SUBI:
            strcpy(strOpcode, "subi");
            break;
        case MULTI:
            strcpy(strOpcode, "multi");
            break;
        case DIVI:
            strcpy(strOpcode, "divi");
            break;        
        case LI:
            strcpy(strOpcode, "li");
            break;        
        case BEQ:
            strcpy(strOpcode, "beq");
            break;
        case BNE:
            strcpy(strOpcode, "bne");
            break;
        case BG:
            strcpy(strOpcode, "bg");
            break;
        case BGE:
            strcpy(strOpcode, "bge");
            break;
        case BL:
            strcpy(strOpcode, "bl");
            break;
        case BLE:
            strcpy(strOpcode, "ble");
            break;
        default:
            break;
    }
    sprintf(str, "%s %d, %d", strOpcode, uf.vj, uf.vk);
    free(strOpcode);
    return str;
}

/* BUFFER LOAD/STORE */
void inicializaBuffer(ComponenteBuffer *buf){
    buf->buffer = (Buffer*)calloc(buf->tamMax, sizeof(Buffer));
    buf->tam = 0;
}

void bufferInsere(ComponenteBuffer *buf, int origem, int destino){
    buf->buffer[buf->tam].origem = origem;
    buf->buffer[buf->tam].destino = destino;
    buf->buffer[buf->tam].busy = true;
    buf->tam++;
}

int procuraBuffer(ComponenteBuffer buf){
   int i;
   for(i = 0; i < buf.tamMax; i++){
       if(buf.buffer[i].busy == false){
           return i;
       }
   }
   
   return -1;
}

bool bufferCheio(ComponenteBuffer buf){
    return buf.tam == buf.tamMax;
}


/* REGISTRADOR */
void inicializaRegistrador(Registrador reg[], int tamFila){
    int i;
    for(i = 0; i < NUM_REGISTRADOR; i++){
        reg[i].qi = criaFila(tamFila);
    }
}

void insereFilaRegistrador(Registrador reg[], int posicao, int estacao){
    filaInsere(reg[posicao].qi, estacao);
}