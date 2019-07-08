#ifndef MENU_H
#define MENU_H

#include "stdlibs.h"
#include "state.h"

#define SAIR 0
#define NRO_OPCOES 4
#define TAM_STRING_VIEW 25

typedef void (*funcao)(state*);

int opcao_valida(int);
int get_opcao(void);
char* get_menu_info(void);
funcao* get_menu_jump_table(void);
void menu(void);

void opcao_1(state*);
void opcao_2(state*);
void opcao_3(state*);

#endif
