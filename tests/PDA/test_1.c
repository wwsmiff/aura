#include "aura/pda.h"
#include "aura/stack.h"

int main() {
  aura_PDA_Machine_t *pda_machine = aura_PDA_Machine_create();

  aura_PDA_Machine_add_state(pda_machine, "q0");
  aura_PDA_Machine_add_state(pda_machine, "q1");

  aura_PDA_Machine_set_input(pda_machine, "ab");

  aura_state_set_type(aura_PDA_Machine_get_state(pda_machine, "q0"),
                      AURA_STATE_INITIAL | AURA_STATE_GENERAL);
  aura_state_set_type(aura_PDA_Machine_get_state(pda_machine, "q1"),
                      AURA_STATE_GENERAL);

  aura_PDA_Machine_set_action(pda_machine, "q0", 'a', AURA_STACK_EMPTY_TOP,
                              "q0", AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, "q0", 'a', 'a', "q0", AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, "q0", 'b', 'a', "q1", AURA_PDA_POP);
  aura_PDA_Machine_set_action(pda_machine, "q1", 'b', 'a', "q1", AURA_PDA_POP);

  aura_PDA_Machine_run(pda_machine, "aaabbb");
  aura_PDA_Machine_run(pda_machine, "aaab");
  aura_PDA_Machine_run(pda_machine, "aabbb");

  aura_PDA_Machine_destroy(pda_machine);

  return 0;
}