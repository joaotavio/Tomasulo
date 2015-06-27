#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "util.h"

void limpar_stdin(){
    while (getchar() != '\n');
}

void pressEnter(){
    printf("\nPressione ENTER para continuar...\n");
    getchar();
}

void clrscr(){
    int i;
    for (i = 0; i < 30; ++i)
        printf("\n\n");
}

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