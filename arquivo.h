#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "tomasulo.h"
#include "instrucao.h"

bool decodificaComponente(char str[], int valor);
bool lerCabecalhoArquitetura(FILE* arquivo);
bool decodificaCiclo(char str[], int valor);
bool lerCabecalhoCiclos(FILE* arquivo);
bool lerCabecalho(FILE* arquivo);
Instrucao decodificaString(char str[]);
bool lerInstrucoes(FILE* arquivo);
bool lerArquivo(FILE* arquivo);

#endif