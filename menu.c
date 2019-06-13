#include "menu.h"

funcao* get_menu_jump_table(void)
{
  static funcao jump_table[] = {
    gerar_arquivo_invertido,
  };
  return jump_table;
}

char* get_menu_info(void)
{
  static char info[] =
"\
0 - SAIR\n\
1 - GERAR ARQUIVO INVERTIDO\n\
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
  int opcao;
  funcao* jump_table = get_menu_jump_table();
  char* menu_info = get_menu_info();
  do
  {
    printf("%s", menu_info);
    opcao = get_opcao();
    
    if (opcao_valida(opcao))
    {
      jump_table[opcao - 1]();
    }
    else if (opcao != SAIR)
    {
      printf("Opção Inválida\n");
    }
    else break;
  }
  while(0);
}