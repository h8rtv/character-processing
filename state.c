#include "state.h"

state* criar_state(void)
{
  state* s = (state*) malloc(sizeof(state));
  if (s == NULL)
  {
    printf("Erro ao criar state.\n");
    return NULL;
  }
  s->arqv_invert = NULL;
  s->buffer = NULL;
  s->buffer_size = 0;
  return s;
}

void destruir_state(state* s)
{
  destruir_arquivo_invertido(s->arqv_invert);
  destruir_buffer(s->buffer);
  s->buffer_size = 0;
  free(s);
}