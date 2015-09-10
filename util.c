#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "util.h"

void strMinuscula(char str[]){
    int i;
    for (i = 0; i < strlen(str); ++i)
        str[i] = tolower(str[i]);
}

bool isNumero(char str[]){
    int i = 0;
    if (str[0] == '-')
        i = 1;
    
    while (i < strlen(str)){
        if (!isdigit(str[i]))
            return false;
        i++;
    }
    
    return true;
}

int calculaBits(int64_t valor){
    if (valor < 0)
        return -1;

    if (valor == 0)
        return 1;

    return (int)floor(log2(valor))+1;
}

void binarioParaString(int64_t valor, char str[]){
    int i = 0;

    if (valor == 0){
        strcpy(str, "0");
        return;
    }
    if (valor < 0){
        valor = -valor;
        str[0] = '-';
        i++;
    }

    i += calculaBits(valor);
    str[i] = '\0';
    while (valor > 0) {
        i--;
        if (valor & 1)
            str[i] = '1';
        else
            str[i] = '0';
        valor >>= 1;
    }
}
