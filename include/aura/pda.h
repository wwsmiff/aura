#ifndef AURA_PDA_H_
#define AURA_PDA_H_

#include "aura/stack.h"
#include "aura/state.h"
#include "aura/string.h"

#define MAX_STATES 512

typedef struct {
  aura_State_t *states[MAX_STATES];
  aura_State_t *current_state;
  aura_String_t input;
  aura_Stack_t stack;

} aura_PDA_Machine_t;

aura_PDA_Machine_t *aura_PDA_Machine_create();
void aura_PDA_Machine_destroy(aura_PDA_Machine_t *machine);

#endif // AURA_PDA_H_