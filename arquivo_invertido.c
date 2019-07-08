#include "arquivo_invertido.h"

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
  if (arqv_invert != NULL)
  {
    for (int i = 0; i < arqv_invert->count_vocabulario; i++)
    {
      free(arqv_invert->itens[i].ocorrencias);
      free(arqv_invert->itens[i].ocorrencias_real);
      free(arqv_invert->itens[i].palavra);
    }
    free(arqv_invert->itens);
    free(arqv_invert);
  }
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

int finaliza_palavra(char c)
{
  return c == ' '  ||
         c == '\n' ||
         c == '\r' ||
         c == ','  ||
         c == '.'  ||
         c == '!'  ||
         c == '?'  ||
         c == ':'  ||
         c == ';';
}

int preencher_arquivo_invertido(arquivo_invertido* arqv_invert, char* buffer, long buffer_size)
{
  int j;
  int i = 0;
  int size_char = 1;
  char palavra[TAM_PALAVRA];
  int count;
  int count_palavra = 0;
  int count_palavra_real = 0;
  int texto_passado_real = 0;
  while (i < buffer_size)
  {
    if (finaliza_palavra(buffer[i]))
    {
      if (count_palavra != 0)
      {
        palavra[count_palavra++] = '\0';
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
          arqv_invert->itens[count].palavra = (char*) malloc(sizeof(char) * count_palavra);
          memcpy(arqv_invert->itens[count].palavra, palavra, count_palavra);
          arqv_invert->itens[count].ocorrencias = (int*) malloc(sizeof(int) * TAM_OCORRENCIAS);
          arqv_invert->itens[count].ocorrencias_real = (int*) malloc(sizeof(int) * TAM_OCORRENCIAS);
          arqv_invert->itens[count].ocorrencias_aloc = TAM_OCORRENCIAS;
          arqv_invert->itens[count].count_ocorrencias = 1;
          arqv_invert->itens[count].ocorrencias[0] = texto_passado_real - count_palavra_real + 1;
          arqv_invert->itens[count].ocorrencias_real[0] = i - count_palavra + 1;
        }
        else
        {
          count = item->count_ocorrencias++;
          if (item->count_ocorrencias > item->ocorrencias_aloc)
          {
            item->ocorrencias_aloc *= 2;
            item->ocorrencias = (int*) realloc(item->ocorrencias, sizeof(int) * item->ocorrencias_aloc);
            item->ocorrencias_real = (int*) realloc(item->ocorrencias_real, sizeof(int) * item->ocorrencias_aloc);
          }
          if (item->ocorrencias == NULL)
          {
            return ERROR;
          }
          item->ocorrencias[count] = texto_passado_real - count_palavra_real + 1;
          item->ocorrencias_real[count] = i - count_palavra + 1;
        }
      }
      count_palavra = 0;
      count_palavra_real = 0;
      i++;
      texto_passado_real++;
    }
    else if (eh_caracter_valido(buffer, i, &size_char))
    {
      for (j = 0; j < size_char; j++)
      {
        palavra[count_palavra + j] = buffer[i++];
      }
      count_palavra += j;
      count_palavra_real++;
      texto_passado_real++;
    }
    else
    {
      i++;
      texto_passado_real++;
    }
      
  }
  return SUCCESS;
}

void gerar_arquivo_invertido(arquivo_invertido* arqv_invert, char* buffer, long buffer_size)
{
  if (preencher_arquivo_invertido(arqv_invert, buffer, buffer_size) == ERROR)
  {
    printf("Erro ao gerar arquivo invertido.\n");
  }
}

int eh_caracter_valido(char* buffer, int pos, int* size_char)
{
  int count_bytes_valido = 1;
  int is_valid;
  char* VALID_X_CHAR = "áàãâéẽêíìóôúÁÀÃÂÉẼÊÍÌÓÔÚç";
  int SIZE_VALID_X_CHAR = strlen(VALID_X_CHAR);
  *size_char = count_bytes_utf8(buffer, pos);

  if (*size_char == 1)
  {
    return buffer[pos] >= 'A' && buffer[pos] <= 'Z' ||
           buffer[pos] >= 'a' && buffer[pos] <= 'z' ||
           buffer[pos] == '-' && buffer[pos + 1] != ' ' && buffer[pos - 1] != ' ';
  }


  for (int j = 0; j < SIZE_VALID_X_CHAR; j += count_bytes_valido)
  {
    for (int i = pos; i < pos + *size_char; i++)
    {
      count_bytes_valido = count_bytes_utf8(VALID_X_CHAR, j);
      is_valid = 1;
      for (int k = j; k < j + count_bytes_valido; k++)
      {
        if (VALID_X_CHAR[k] != buffer[i])
        {
          is_valid = 0;
          break;
        }
        else
        {
          i++;
        }
      }
      if (is_valid)
      {
        return 1;
      }
    }
  }
  return 0;
}

int count_bytes_utf8(char* buffer, int pos)
{
  if (buffer[pos] >> 7 == 0)
  {
    return 1;
  }
  else
  {
    unsigned char cmp_byte = 1 << 7;
    int count_bytes = 1;
    while (buffer[pos] & cmp_byte)
    {
      ++count_bytes;
      cmp_byte >>= 1;
    }
    return count_bytes - 1;
  }
}

void print_arquivo_invertido(arquivo_invertido* arqv_invert)
{
  if (arqv_invert == NULL)
  {
    printf("O arquivo invertido não foi carregado.\n");
    return;
  }
  for (int i = 0; i < arqv_invert->count_vocabulario; i++, putchar('\n'))
  {
    printf("%s ", arqv_invert->itens[i].palavra);
    for (int j = 0; j < arqv_invert->itens[i].count_ocorrencias; j++)
    {
      printf("%d ", arqv_invert->itens[i].ocorrencias[j]);
    }
  }
}