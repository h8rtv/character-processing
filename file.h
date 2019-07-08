#ifndef FILE_H
#define FILE_H

#include "stdlibs.h"

#define ERROR 1
#define SUCCESS 0

#define TAM_VOCABULARIO 100
#define TAM_PALAVRA 50
#define TAM_OCORRENCIAS 2

FILE* ler_arquivo(const char*);
void fechar_arquivo(FILE*);
long get_tamanho_arquivo(FILE*);
int preencher_file_buffer(FILE*, char*, long);
char* gerar_buffer(const char*, long*);
void destruir_buffer(char*);

#endif
