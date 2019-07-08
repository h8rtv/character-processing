#ifndef ARQUIVO_INVERTIDO_H
#define ARQUIVO_INVERTIDO_H

#include "stdlibs.h"

#define TAM_VOCABULARIO 100
#define TAM_PALAVRA 50
#define TAM_OCORRENCIAS 2

struct _item_arquivo_invertido
{
  char* palavra;
  unsigned count_ocorrencias;
  unsigned ocorrencias_aloc;
  int* ocorrencias;
  int* ocorrencias_real;
};
typedef struct _item_arquivo_invertido item_arquivo_invertido;

struct _arquivo_invertido
{
  unsigned count_vocabulario;
  unsigned itens_aloc;
  item_arquivo_invertido* itens;
};
typedef struct _arquivo_invertido arquivo_invertido;

int preencher_arquivo_invertido(arquivo_invertido*, char*, long);
item_arquivo_invertido* procurar_palavra(arquivo_invertido*, char*);
void gerar_arquivo_invertido(arquivo_invertido*, char*, long);
arquivo_invertido* criar_arquivo_invertido();
int count_bytes_utf8(char*, int);
int eh_caracter_valido(char*, int, int*);
void print_arquivo_invertido(arquivo_invertido*);
void destruir_arquivo_invertido(arquivo_invertido*);

#endif
