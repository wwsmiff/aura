/*
 * Define a PDA for ensuring equal number of 'a's followed by an equal number of
 * 'b's
 */

#include "aura/pda.h"
#include "aura/stack.h"
#include <stdio.h>

int main() {
  aura_PDA_Machine_t *pda_machine = aura_PDA_Machine_create();
  aura_PDA_Machine_add_state(pda_machine, AURA_STRING_LITERAL("q0")); // Initial
  aura_PDA_Machine_add_state(pda_machine, AURA_STRING_LITERAL("q1"));
  aura_PDA_Machine_add_state(pda_machine, AURA_STRING_LITERAL("q2"));
  aura_PDA_Machine_add_state(pda_machine, AURA_STRING_LITERAL("q3")); // Final
  aura_PDA_Machine_add_state(pda_machine,
                             AURA_STRING_LITERAL("q4")); // Unaccpeted

  aura_PDA_Machine_set_input(pda_machine, AURA_STRING_LITERAL("ab"));

  aura_state_set_type(
      aura_PDA_Machine_get_state(pda_machine, AURA_STRING_LITERAL("q0")),
      AURA_STATE_INITIAL | AURA_STATE_GENERAL);
  aura_state_set_type(
      aura_PDA_Machine_get_state(pda_machine, AURA_STRING_LITERAL("q3")),
      AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q0"), 'a',
                              EPSILON, AURA_STRING_LITERAL("q1"),
                              AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q0"), 'b',
                              EPSILON, AURA_STRING_LITERAL("q4"),
                              AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q1"), 'a', 'a',
                              AURA_STRING_LITERAL("q1"), AURA_PDA_PUSH);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q1"), 'b', 'a',
                              AURA_STRING_LITERAL("q2"), AURA_PDA_POP);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q2"), 'b', 'a',
                              AURA_STRING_LITERAL("q2"), AURA_PDA_POP);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q2"), 'a', 'a',
                              AURA_STRING_LITERAL("q4"), AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q2"), 'b',
                              EPSILON, AURA_STRING_LITERAL("q4"),
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q2"), EPSILON,
                              EPSILON, AURA_STRING_LITERAL("q3"),
                              AURA_PDA_NO_OPERATION);

  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q4"), 'a', 'a',
                              AURA_STRING_LITERAL("q4"), AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q4"), EPSILON,
                              'a', AURA_STRING_LITERAL("q4"),
                              AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q4"), 'b', 'a',
                              AURA_STRING_LITERAL("q4"), AURA_PDA_NO_OPERATION);
  aura_PDA_Machine_set_action(pda_machine, AURA_STRING_LITERAL("q4"), EPSILON,
                              EPSILON, AURA_STRING_LITERAL("q4"),
                              AURA_PDA_NO_OPERATION);

  aura_PDA_Machine_run(pda_machine, AURA_STRING_LITERAL("aaabbb"));
  aura_PDA_Machine_run(pda_machine, AURA_STRING_LITERAL("aaabb"));
  aura_PDA_Machine_run(pda_machine, AURA_STRING_LITERAL("aabbb"));

  aura_PDA_Machine_destroy(pda_machine);

  return 0;
}
