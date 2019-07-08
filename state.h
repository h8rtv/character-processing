#ifndef STATE_H
#define STATE_H

#include "arquivo_invertido.h"
#include "file.h"

struct _state
{
  long buffer_size;
  char* buffer;
  arquivo_invertido* arqv_invert;
};
typedef struct _state state;

state* criar_state(void);
void destruir_state(state*);

#endif
