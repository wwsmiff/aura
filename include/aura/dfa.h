#ifndef AURA_DFA_H_
#define AURA_DFA_H_

#include "aura/error.h"
#include "aura/state.h"
#include "aura/string.h"

#define MAX_STATES 512

typedef struct {
  aura_String_t src;
  aura_State_t *dest;
  char trigger;
} aura_Path_t;

typedef struct {
  aura_State_t *states[MAX_STATES];
  aura_Path_t **paths;
  aura_State_t *current_state;
  aura_String_t input;
  size_t active_states;
  size_t path_len;
} aura_DFA_Machine_t;

aura_DFA_Machine_t *aura_DFA_Machine_create();
void aura_DFA_Machine_destroy(aura_DFA_Machine_t *machine);

void aura_DFA_Machine_set_input(aura_DFA_Machine_t *machine,
                                aura_String_t input);
void aura_DFA_Machine_add_state(aura_DFA_Machine_t *machine,
                                aura_String_t label);

void aura_DFA_Machine_set_path(aura_DFA_Machine_t *machine, aura_String_t src,
                               char trigger, aura_String_t dest);

aura_RuntimeErrorType aura_DFA_Machine_run(aura_DFA_Machine_t *machine,
                                           aura_String_t input);

aura_State_t *aura_DFA_Machine_get_state(aura_DFA_Machine_t *machine,
                                         aura_String_t label);

#endif // AURA_DFA_H_
