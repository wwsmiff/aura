#include "aura/pda.h"

int main() {
  aura_PDA_Machine_t *pda_machine = aura_PDA_Machine_create();

  aura_PDA_Machine_add_state(pda_machine, "q0");
  aura_PDA_Machine_add_state(pda_machine, "q1");
  aura_PDA_Machine_add_state(pda_machine, "q2");

  aura_state_set_type(aura_PDA_Machine_get_state(pda_machine, "q0"),
                      AURA_STATE_GENERAL | AURA_STATE_INITIAL);

  aura_PDA_Machine_set_input(pda_machine, "ab");

  aura_PDA_Machine_set_action(pda_machine, "q0", 'a', AURA_STACK_EMPTY_TOP,
                              "q1", AURA_PDA_PUSH);

  aura_PDA_Machine_set_action(pda_machine, "q1", 'a', 'a', "q1", AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, "q1", 'b', 'a', "q2", AURA_PDA_POP);
  aura_PDA_Machine_set_action(pda_machine, "q2", 'b', 'a', "q1",
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, "q2", 'b', AURA_STACK_EMPTY_TOP,
                              "q2", AURA_PDA_NO_OPERATION);

  aura_PDA_Machine_run(pda_machine, "abb");
  aura_PDA_Machine_run(pda_machine, "aabb");
  aura_PDA_Machine_run(pda_machine, "aabbbb");
  aura_PDA_Machine_run(pda_machine, "aaaabb");

  aura_PDA_Machine_destroy(pda_machine);

  return 0;
}