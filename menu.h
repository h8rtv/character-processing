#ifndef MENU_H
#define MENU_H

#include "stdlibs.h"
#include "arquivo_invertido.h"

#define SAIR 0
#define NRO_OPCOES 2

typedef void (*funcao)(void);

int opcao_valida(int);
int get_opcao(void);
char* get_menu_info(void);
funcao* get_menu_jump_table(void);
void menu(void);

#endif
