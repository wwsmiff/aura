/*
 * Define a PDA for ensuring equal number of 'a's followed by an equal number of
 * 'b's
 */

#include "aura/pda.h"
#include "aura/stack.h"
#include <stdio.h>

int main() {
  aura_PDA_Machine_t *pda_machine = aura_PDA_Machine_create();
  aura_PDA_Machine_add_state(pda_machine, "q0"); // Initial
  aura_PDA_Machine_add_state(pda_machine, "q1");
  aura_PDA_Machine_add_state(pda_machine, "q2");
  aura_PDA_Machine_add_state(pda_machine, "q3"); // Final
  aura_PDA_Machine_add_state(pda_machine, "q4"); // Unaccpeted

  aura_PDA_Machine_set_input(pda_machine, "ab");

  aura_state_set_type(aura_PDA_Machine_get_state(pda_machine, "q0"),
                      AURA_STATE_INITIAL | AURA_STATE_GENERAL);
  aura_state_set_type(aura_PDA_Machine_get_state(pda_machine, "q3"),
                      AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_PDA_Machine_set_action(pda_machine, "q0", 'a', EPSILON, "q1",
                              AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, "q0", 'b', EPSILON, "q4",
                              AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, "q1", 'a', 'a', "q1", AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, "q1", 'b', 'a', "q2", AURA_PDA_POP);
  aura_PDA_Machine_set_action(pda_machine, "q2", 'b', 'a', "q2", AURA_PDA_POP);
  aura_PDA_Machine_set_action(pda_machine, "q2", 'a', 'a', "q4",
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, "q2", 'b', EPSILON, "q4",
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, "q2", EPSILON, EPSILON, "q3",
                              AURA_PDA_NO_OPERATION);

  aura_PDA_Machine_set_action(pda_machine, "q4", 'a', 'a', "q4",
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, "q4", EPSILON, 'a', "q4",
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, "q4", 'b', 'a', "q4",
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, "q4", EPSILON, EPSILON, "q4",
                              AURA_PDA_NO_OPERATION);

  aura_PDA_Machine_run(pda_machine, "aaabbb");
  aura_PDA_Machine_run(pda_machine, "aaabb");
  aura_PDA_Machine_run(pda_machine, "aabbb");

  return 0;
}
