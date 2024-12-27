#ifndef AURA_PDA_H_
#define AURA_PDA_H_

#include "aura/stack.h"
#include "aura/state.h"
#include "aura/string.h"

#define MAX_STATES 512

typedef enum {
  AURA_PDA_NO_OPERATION,
  AURA_PDA_PUSH,
  AURA_PDA_POP
} aura_PDAOperationType;

typedef struct {
  aura_String_t src;
  char trigger;
  char stack_top;
  aura_State_t *dest;
  aura_PDAOperationType op;
} aura_PDA_Action_t;

typedef struct {
  aura_State_t *states[MAX_STATES];
  aura_State_t *current_state;
  aura_String_t input;
  aura_Stack_t stack;
  aura_PDA_Action_t **actions;
  size_t action_len;
  size_t active_states;
} aura_PDA_Machine_t;

aura_PDA_Machine_t *aura_PDA_Machine_create();
void aura_PDA_Machine_destroy(aura_PDA_Machine_t *machine);

void aura_PDA_Machine_set_input(aura_PDA_Machine_t *machine, const char *input);
void aura_PDA_Machine_add_state(aura_PDA_Machine_t *machine, const char *label);
aura_State_t *aura_PDA_Machine_get_state(aura_PDA_Machine_t *machine,
                                         const char *label);
void aura_PDA_Machine_set_action(aura_PDA_Machine_t *machine, const char *src,
                                 char trigger, char stack_top, const char *dest,
                                 aura_PDAOperationType op);

aura_PDA_Action_t *aura_PDA_Machine_get_action(aura_PDA_Machine_t *machine,
                                               const char *src, char trigger,
                                               char stack_top);

void aura_PDA_Machine_run(aura_PDA_Machine_t *machine, const char *input);

#endif // AURA_PDA_H_
