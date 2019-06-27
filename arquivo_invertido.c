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
  arquivo_invertido* arqv_invert = (arquivo_invertido*) malloc(sizeof(arquivo_invertido));

  if (arqv_invert != NULL)
  {
    arqv_invert->count_vocabulario = 0;
    arqv_invert->itens = (item_arquivo_invertido*) malloc(sizeof(item_arquivo_invertido) * TAM_VOCABULARIO);
    arqv_invert->itens_aloc = TAM_VOCABULARIO;
  }
}

void destruir_arquivo_invertido(arquivo_invertido* arqv_invert)
{
  free(arqv_invert->itens);
  free(arqv_invert);
}

item_arquivo_invertido* procurar_palavra(arquivo_invertido* arqv_invert, char* palavra)
{
  for (int i = 0; i < arqv_invert->count_vocabulario; i++)
  {
    if (strcmp(arqv_invert->itens[i].palavra, palavra) == 0)
    {
      return &arqv_invert->itens[i];
    }
  }
  return NULL;
}

int preencher_arquivo_invertido(arquivo_invertido* arqv_invert, char* buffer, long buffer_size)
{
  int size_char = 1;
  char palavra[TAM_PALAVRA];
  int count;
  int count_palavra = 0;
  for (int i = 0; i < buffer_size; i += size_char)
  {
    if (buffer[i] == ' ')
    {
      if (strlen(palavra) != 0)
      {
        palavra[count_palavra] = '\0';
        item_arquivo_invertido* item = procurar_palavra(arqv_invert, palavra);
        if (item == NULL)
        {
          count = arqv_invert->count_vocabulario++;
          if (arqv_invert->count_vocabulario > arqv_invert->itens_aloc)
          {
            arqv_invert->itens_aloc *= 2;
            arqv_invert->itens = (item_arquivo_invertido*) realloc(arqv_invert->itens, sizeof(item_arquivo_invertido) * arqv_invert->itens_aloc);
          }
          if (arqv_invert->itens == NULL)
          {
            return ERROR;
          }
          memcpy(arqv_invert->itens[count].palavra, palavra, count_palavra);
          arqv_invert->itens[count].ocorrencias = (int*) malloc(sizeof(int) * TAM_OCORRENCIAS);
          arqv_invert->itens[count].ocorrencias_aloc = TAM_OCORRENCIAS;
          arqv_invert->itens[count].count_ocorrencias = 1;
          arqv_invert->itens[count].ocorrencias[0] = i - count_palavra;
        }
        else
        {
          count = item->count_ocorrencias++;
          if (item->count_ocorrencias > item->ocorrencias_aloc)
          {
            item->ocorrencias_aloc *= 2;
            item->ocorrencias = (int*) realloc(item->ocorrencias, sizeof(int) * item->ocorrencias_aloc);
          }
          if (item->ocorrencias == NULL)
          {
            return ERROR;
          }
          item->ocorrencias[count] = i - count_palavra;
        }
      }
      count_palavra = 0;
    }
    else if (eh_caracter_valido(buffer, i, &size_char))
    {
      palavra[count_palavra++] = buffer[i];
    }
  }
  return SUCCESS;
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

  arquivo_invertido* arqv_invert = criar_arquivo_invertido();

  if (preencher_arquivo_invertido(arqv_invert, buffer, buffer_size) == ERROR)
  {
    printf("Erro ao gerar arquivo invertido.\n");
    return;
  }

  // for (int i = 0; i < arqv_invert->count_vocabulario; i++, putchar('\n'))
  // {
  //   printf("%s ", arqv_invert->itens[i].palavra);
  //   for (int j = 0; j < arqv_invert->itens[i].count_ocorrencias; j++)
  //   {
  //     printf("%d ", arqv_invert->itens[i].ocorrencias[j]);
  //   }
  // }

  free(buffer);
  destruir_arquivo_invertido(arqv_invert);
}

int eh_caracter_valido(char* buffer, int pos, int* size_char)
{
  int count_bytes_valido;
  char* CARACTERES_ESPECIAIS_VALIDOS = "áàãâéẽêíìóôú";

  *size_char = count_bytes_utf8(buffer, pos);

  for (int i = pos; i < pos + *size_char; i++)
  {
    for (int j = 0; j < 12; j += count_bytes_valido)
    {
      count_bytes_valido = count_bytes_utf8(CARACTERES_ESPECIAIS_VALIDOS, j);
      for (int k = j; k < j + count_bytes_valido; k++)
      {
        if (CARACTERES_ESPECIAIS_VALIDOS[k] != buffer[i])
        {
          return 0;
        }
      }
    }
  }
  return 1;
}

int count_bytes_utf8(char* buffer, int pos)
{
  if (buffer[pos] >> 7 == 0)
  {
    return 0;
  }
  else
  {
    char cmp_byte = 128;
    int count_bytes = 1;
    while (buffer[pos] & cmp_byte)
    {
      ++count_bytes;
      cmp_byte >>= 1;
    }
    return count_bytes - 1;
  }
}
