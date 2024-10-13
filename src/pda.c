#include "aura/pda.h"
#include <stdlib.h>

aura_PDA_Machine_t *aura_PDA_Machine_create() {
  aura_PDA_Machine_t *machine =
      (aura_PDA_Machine_t *)(malloc(sizeof(aura_PDA_Machine_t)));

  for (size_t i = 0; i < MAX_STATES; ++i) {
    machine->states[i] = NULL;
  }

  machine->current_state = NULL;
  machine->input = aura_string_create();
  machine->stack = aura_stack_create();
  return machine;
}

void aura_PDA_Machine_destroy(aura_PDA_Machine_t *machine) {
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      aura_state_destroy(machine->states[i]);
      machine->states[i] = NULL;
    }
  }
  aura_string_destroy(&machine->input);
  aura_stack_destroy(&machine->stack);
  free(machine);
}
