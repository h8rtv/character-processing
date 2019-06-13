#include "arquivo_invertido.h"


FILE* ler_arquivo(void)
{
  return fopen("Historia.txt", "r");
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

arquivo_invertido* criar_arquivo_invertido(void)
{
  arquivo_invertido* arqv_invertido = (arquivo_invertido*) malloc(sizeof(arquivo_invertido));
  
  if (arqv_invertido != NULL)
  {
    arqv_invertido->count_vocabulario = 0;
    arqv_invertido->itens = (item_arquivo_invertido*) malloc(sizeof(item_arquivo_invertido) * TAM_VOCABULARIO);
    arqv_invertido->itens_alocados = TAM_VOCABULARIO;
  }
}

void destruir_arquivo_invertido(arquivo_invertido* arqv_invertido)
{
  free(arqv_invertido->itens);
  free(arqv_invertido);
}

void gerar_arquivo_invertido(void)
{
  FILE* arquivo = ler_arquivo();
  if (arquivo == NULL)
  {
    printf("Erro ao ler o arquivo.\n");
    return;
  }

  long buffer_size = get_tamanho_arquivo(arquivo);
  if (buffer_size == ERROR)
  {
    printf("Erro ao contar o tamanho do arquivo.\n");
    return; 
  }

  char* buffer = (char*) malloc(sizeof(char) * buffer_size + 1);
  if (buffer == NULL)
  {
    printf("Erro ao alocar buffer.\n");
    return;
  }

  if (preencher_file_buffer(arquivo, buffer, buffer_size) == ERROR)
  {
    printf("Erro ao preencher buffer.\n");
    return;
  }

  fechar_arquivo(arquivo);

  arquivo_invertido* arqv_invertido = criar_arquivo_invertido();
  
  char palavra[TAM_PALAVRA];
  int count_vocabulario;
  int count_palavra = 0;
  for (int i = 0; i < buffer_size; i++)
  {
    if (buffer[i] == ' ')
    {
      if (strlen(palavra) != 0)
      {
        count_vocabulario = arqv_invertido->count_vocabulario++;
        if (arqv_invertido->count_vocabulario > arqv_invertido->itens_alocados)
        {
          arqv_invertido->itens_alocados *= 2;
          arqv_invertido->itens = realloc(arqv_invertido->itens, sizeof(item_arquivo_invertido) * arqv_invertido->itens_alocados);
        }
        if (arqv_invertido->itens == NULL)
        {
          printf("Erro ao gerar arquivo invertido.\n");
          return;
        }
        memcpy(arqv_invertido->itens[count_vocabulario].palavra, palavra, count_palavra);
      }
      count_palavra = 0;
    }
    else
    {
      palavra[count_palavra++] = buffer[i];
    }
  }

  for (int i = 0; i < arqv_invertido->count_vocabulario; i++)
  {
    printf("%s \n", arqv_invertido->itens[i].palavra);
  }
  free(buffer);
  destruir_arquivo_invertido(arqv_invertido);
}
