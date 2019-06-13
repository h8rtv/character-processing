#ifndef ARQUIVO_INVERTIDO_H
#define ARQUIVO_INVERTIDO_H

#include "stdlibs.h"

#define ERROR 1
#define SUCCESS 0

#define TAM_VOCABULARIO 500
#define TAM_PALAVRA 35
#define TAM_OCORRENCIAS 10

struct _item_arquivo_invertido
{
  char palavra[TAM_PALAVRA];
  int count_ocorrencias;
  int ocorrencias[TAM_OCORRENCIAS];
};
typedef struct _item_arquivo_invertido item_arquivo_invertido;

struct _arquivo_invertido
{
  unsigned count_vocabulario;
  unsigned itens_alocados;
  item_arquivo_invertido* itens;
};
typedef struct _arquivo_invertido arquivo_invertido;


FILE* ler_arquivo(void);
void fechar_arquivo(FILE*);
long get_tamanho_arquivo(FILE*);
int preencher_file_buffer(FILE*, char*, long);

void gerar_arquivo_invertido(void);

#endif
