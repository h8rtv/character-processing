#include "file.h"

FILE* ler_arquivo(const char* nome_arquivo)
{
  return fopen(nome_arquivo, "r");
}

void fechar_arquivo(FILE* arquivo)
{
  fclose(arquivo);
}

long get_tamanho_arquivo(FILE* arquivo)
{
  if (fseek(arquivo, 0L, SEEK_END) != SUCCESS)
  {
    return ERROR;
  }

  long buffer_size = ftell(arquivo);
  if (fseek(arquivo, 0L, SEEK_SET) != SUCCESS)
  {
    return ERROR;
  }
  return buffer_size;
}

int preencher_file_buffer(FILE* arquivo, char* buffer, long buffer_size)
{
  size_t length = fread(buffer, sizeof(char), buffer_size, arquivo);
  if (ferror(arquivo))
  {
    return ERROR;
  }
  else
  {
    buffer[length++] = '\0';
    return SUCCESS;
  }
}

char* gerar_buffer(const char* nome_arquivo, long* buffer_size)
{
  FILE* arquivo = ler_arquivo(nome_arquivo);
  if (arquivo == NULL)
  {
    printf("Erro ao ler o arquivo.\n");
    return NULL;
  }

  *buffer_size = get_tamanho_arquivo(arquivo);
  if (*buffer_size == ERROR)
  {
    printf("Erro ao contar o tamanho do arquivo.\n");
    return NULL;
  }

  char* buffer = (char*) malloc(sizeof(char) * (*buffer_size) + 1);
  if (buffer == NULL)
  {
    printf("Erro ao alocar buffer.\n");
    return NULL;
  }

  if (preencher_file_buffer(arquivo, buffer, *buffer_size) == ERROR)
  {
    printf("Erro ao preencher buffer.\n");
    return NULL;
  }

  fechar_arquivo(arquivo);
  
  return buffer;
}

void destruir_buffer(char* buffer)
{
  free(buffer);
}