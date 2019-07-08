#include "menu.h"

int max(int a, int b)
{
  return a > b ? a : b;
}

funcao* get_menu_jump_table(void)
{
  static funcao jump_table[] = {
    opcao_1,
    opcao_2,
    opcao_3,
  };
  return jump_table;
}

char* get_menu_info(void)
{
  static char info[] =
"\
0 - SAIR\n\
1 - GERAR ARQUIVO INVERTIDO\n\
2 - IMPRIMIR ARQUIVO INVERTIDO\n\
3 - PROCURAR NO ARQUIVO INVERTIDO\n\
";
  return info;
}

int opcao_valida(int opcao)
{
  return opcao > 0 && opcao < NRO_OPCOES;
}

int get_opcao(void)
{
  int opcao;
  scanf("%d", &opcao);
  return opcao;
}

void menu(void)
{
  int opcao = SAIR;
  funcao* jump_table = get_menu_jump_table();
  char* menu_info = get_menu_info();
  state* s = criar_state();
  s->buffer = gerar_buffer("Historia.txt", &s->buffer_size);
  do
  {
    printf("\n%s\n", menu_info);
    opcao = get_opcao();

    if (opcao_valida(opcao))
    {
      jump_table[opcao - 1](s);
    }
    else if (opcao != SAIR)
    {
      printf("Opção Inválida\n");
    }
    else
    {
      destruir_state(s);
      break;
    }
  }
  while(1);
}

void opcao_1(state* s)
{
  if (s->arqv_invert  == NULL)
  {
    s->arqv_invert = criar_arquivo_invertido();
  }
  else
  {
    destruir_arquivo_invertido(s->arqv_invert);
    s->arqv_invert = criar_arquivo_invertido();
  }
  
  gerar_arquivo_invertido(s->arqv_invert, s->buffer, s->buffer_size);
}

void opcao_2(state* s)
{
  print_arquivo_invertido(s->arqv_invert);
}


void opcao_3(state* s)
{
  char palavra[TAM_PALAVRA];
  printf("Digite a palavra que deseja buscar: ");
  if (scanf("%s", palavra) == 0)
  {
    printf("Erro ao ler palavra.\n");
    return;
  }
  item_arquivo_invertido* item = procurar_palavra(s->arqv_invert, palavra);
  if (item == NULL)
  {
    printf("Nenhum resultado encontrado.\n");
  }
  else
  {
    int opcao = SAIR;
    int i = 0;
    do
    {
      printf("Palavra: %s\n", item->palavra);
      printf("N° de ocorrências: %d\n", item->count_ocorrencias);
      printf("Ocorrência atual: %d°\n", i + 1);
      printf("Posição: %d|Posição no buffer: %d\n\n", item->ocorrencias[i] ,item->ocorrencias_real[i]);
      printf("%.*s\n\n", TAM_STRING_VIEW * 2, s->buffer + max(item->ocorrencias_real[i] - TAM_STRING_VIEW, 0));
      printf("0 - Voltar\n");
      printf("1 - Ir para a ocorrência anterior\n");
      printf("2 - Ir para a ocorrência posterior\n");
      scanf("%d", &opcao);

      if (opcao == 1)
      {
        if (i == 0)
        {
          i = item->count_ocorrencias - 1;
        }
        else
        {
          i--;
        }  
      }
      else if (opcao == 2)
      {
        if (i == item->count_ocorrencias - 1)
        {
          i = 0;
        }
        else
        {
          i++;
        }
      }
      else if (opcao != SAIR)
      {
        opcao = SAIR;
      }
    } while (opcao != SAIR);
  }
}
