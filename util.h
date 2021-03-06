#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>
#include <stdint.h>

void limpar_stdin();
void pressEnter();
void clrscr();
void strMinuscula(char str[]);
bool isNumero(char str[]);
int calculaBits(int64_t valor);
void binarioParaString(int64_t valor, char str[]);
void printBinario(int64_t valor);

#endif
