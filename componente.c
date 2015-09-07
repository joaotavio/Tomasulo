#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "componente.h"
#include "tomasulo.h"

Fila fila;
Registrador registrador[NUM_REGISTRADOR];
UnidadeEndereco unidadeEndereco;
ComponenteER er_somador;
ComponenteER er_multiplicador;
ComponenteUF somador;
ComponenteUF multiplicador;
ComponenteBuffer load;
ComponenteBuffer store;
CDB barramento;


/* FILA */
void inicializaFila(int tamanho){
    fila = criaFila(tamanho);
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

void estacaoInsere(ComponenteER *er, int id, Operacoes opcode, int qj, int qk, int vj, int vk, int A, int posicao){
    er->est_reserva[posicao].id = id;
	er->est_reserva[posicao].opcode = opcode;
	er->est_reserva[posicao].qj = qj;
	er->est_reserva[posicao].qk = qk;
    er->est_reserva[posicao].vj = vj;
    er->est_reserva[posicao].vk = vk;
    er->est_reserva[posicao].busy = true;
    er->est_reserva[posicao].A = A;
    er->tam++;
}

void estacaoRemove(ComponenteER *er, int posicao){
    er->est_reserva[posicao].id = -1;
    er->est_reserva[posicao].opcode = NOP;
    er->est_reserva[posicao].qj = -1;
    er->est_reserva[posicao].qk = -1;
    er->est_reserva[posicao].busy = false;
    er->est_reserva[posicao].vj = 0;
    er->est_reserva[posicao].vk = 0;
    er->est_reserva[posicao].A = -1;
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
    char *str = malloc(sizeof(char)*MAX_STR_PRINT);
    char strOpcode[10];
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

    return str;
}

/* UNIDADE FUNCIONAL */
bool unFuncionalCheia(ComponenteUF uf){
    return uf.tam == uf.tamMax;
}

void uFuncionalInsere(ComponenteUF *uf, int posicao, int id, Operacoes opcode, int vj, int vk, int ciclos){
    uf->un_funcional[posicao].id = id;
    uf->un_funcional[posicao].opcode = opcode;
    uf->un_funcional[posicao].vj = vj;
    uf->un_funcional[posicao].vk = vk;
    uf->un_funcional[posicao].busy = true;
    uf->un_funcional[posicao].ciclos = ciclos;
    uf->tam++;
}

void uFuncionalRemove(ComponenteUF *uf, int posicao){
    uf->un_funcional[posicao].id = -1;
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
    char *str = malloc(sizeof(char)*MAX_STR_PRINT);
    char strOpcode[10];
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
    return str;
}

/* BUFFER LOAD/STORE */
void inicializaBuffer(ComponenteBuffer *buf){
    buf->buffer = (Buffer*)calloc(buf->tamMax, sizeof(Buffer));
    buf->tam = 0;
}

void bufferInsere(ComponenteBuffer *buf, Operacoes opcode, int origem, int destino){
    buf->buffer[buf->tam].opcode = opcode;
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

int bufferLivre(ComponenteBuffer buf){
	int i;
	if(!bufferCheio(buf)){
		for(i = 0; i < buf.tamMax; i++){
				if(!buf.buffer[i].busy){
					return i;
				}
		}
	}
	return -1;
}

void bufferRemove(ComponenteBuffer *buf, int posicao){
	buf->buffer[posicao].opcode = NOP;
	buf->buffer[posicao].origem = 0;
	buf->buffer[posicao].destino = 0;
	buf->buffer[posicao].busy = false;
	buf->tam--;
}

char* bfToString(Buffer buffer){
	char *str = malloc(sizeof(char)*MAX_STR_PRINT);
    switch(buffer.opcode){
        case LD:
            sprintf(str, "ld [%d], %d", buffer.origem, buffer.destino);
            break;
        case SD:
            sprintf(str, "sd %d, [%d]", buffer.origem, buffer.destino);
            break;
        default:
            break;
    }

    return str;
}

/*UNIDADE DE ENDERECO*/
void uEnderecoInsere(UnidadeEndereco *ue, Operacoes opcode, int origem, int destino){
	ue->opcode = opcode;
	ue->origem = origem;
	ue->destino = destino;
	ue->busy = true;
}

void uEnderecoRemove(UnidadeEndereco *ue){
	ue->opcode = NOP;
	ue->origem = 0;
	ue->destino = 0;
	ue->busy = false;
}

bool uEnderecoCheia(){
	return unidadeEndereco.busy;
}
	
char* ueToString(UnidadeEndereco ue){
	char *str = malloc(sizeof(char)*MAX_STR_PRINT);
    switch(ue.opcode){
        case LD:
            sprintf(str, "ld [%d], %d", ue.origem, ue.destino);
            break;
        case SD:
            sprintf(str, "sd %d, [%d]", ue.origem, ue.destino);
            break;
        default:
            break;
    }

    return str;
}

/* REGISTRADOR */
void inicializaRegistrador(Registrador reg[]){
    int i;
    for(i = 0; i < NUM_REGISTRADOR; i++){
        reg[i].qi = -1;
    }
}

int codificaEstacao(int posicao, TipoComponente tipo){
    int ret;
    switch (tipo){
        case LOAD:
            ret = posicao;
            break;
        case STORE:
            ret = posicao + load.tamMax;
            break;
        case SOMADOR:
            ret = posicao + load.tamMax + store.tamMax;
            break;
        case MULTIPLICADOR:
            ret = posicao + load.tamMax + store.tamMax + somador.tamMax;
            break;
    }
    return ret;
}

int decodificaEstacao(int posicao){
    int ret;
    if (posicao < store.tamMax)
        ret = posicao;
    else if (posicao < somador.tamMax)
        ret = posicao - load.tamMax;
    else if (posicao < multiplicador.tamMax)
        ret = posicao - load.tamMax + store.tamMax;
    else 
        ret = posicao - load.tamMax + store.tamMax + somador.tamMax;
    
    return ret;
}

void registradorMudaQI(Registrador reg[], int posicao, int id, int estacao, TipoComponente tipo){
    estacao = codificaEstacao(estacao, tipo);
    reg[posicao].qi = estacao;
    reg[posicao].id = id;
}

int registradorObterQI(Registrador reg[], int posicao){
    return reg[posicao].qi;
    //return decodificaEstacao(registrador[posicao].qi);
}

void registradorEscreve(Registrador reg[], int posicao, int64_t valor){
    reg[posicao].valor = valor;
    reg[posicao].qi = -1;
    reg[posicao].id = -1;
}

int procuraRegistrador(Registrador reg[], int estacao, TipoComponente tipo){
    estacao = codificaEstacao(estacao, tipo);
    int i;
    for (i = 0; i < NUM_REGISTRADOR; i++){
        if (reg[i].qi == estacao){
            return i;
        }
    }
    return -1;
}

/* BARRAMENTO */
void inicializaBarramento(CDB *barramento, int tam_load, int tam_store, int tam_somador, int tam_mult){
    barramento->tamMax = tam_load + tam_store + tam_somador + tam_mult;
    barramento->campo = (CampoBarramento*)calloc(barramento->tamMax, sizeof(CampoBarramento));
}

void barramentoInsere(CDB *barramento, int64_t dado, int id, int posicao, TipoComponente tipo){
    posicao = codificaEstacao(posicao, tipo);
    barramento->campo[posicao].dado = dado;
    barramento->campo[posicao].id = id;
}

int barramentoObterDado(CDB barramento, int posicao){
    return barramento.campo[posicao].dado;
}

void barramentoLimpar(CDB *barramento){
    //printf("%d\n", barramento->tamMax*sizeof(CampoBarramento));
   /* int i;
    for (i = 0; i < barramento->tamMax; ++i) {
        barramento->campo[i].dado = -1;
        barramento->campo[i].id = -1;
    }*/
    memset(barramento->campo, -1, barramento->tamMax*sizeof(CampoBarramento));
}