/* Test to define a DFA with accepts inputs
 * {'0', '1'} with consecutive '0's and '1's and
 * starts with a '0'.
 */

#include "aura/dfa.h"

int main() {
  aura_DFA_Machine_t *dfa_machine = aura_DFA_Machine_create();
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q0"));
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q1"));
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q2"));
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q3"));

  aura_DFA_Machine_set_input(dfa_machine, AURA_STRING_LITERAL("01"));

  aura_state_set_type(
      aura_DFA_Machine_get_state(dfa_machine, AURA_STRING_LITERAL("q0")),
      AURA_STATE_INITIAL | AURA_STATE_GENERAL);

  aura_state_set_type(
      aura_DFA_Machine_get_state(dfa_machine, AURA_STRING_LITERAL("q2")),
      AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q0"), '0',
                            AURA_STRING_LITERAL("q1"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q0"), '1',
                            AURA_STRING_LITERAL("q3"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q1"), '1',
                            AURA_STRING_LITERAL("q2"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q1"), '0',
                            AURA_STRING_LITERAL("q3"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q2"), '0',
                            AURA_STRING_LITERAL("q1"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q2"), '1',
                            AURA_STRING_LITERAL("q3"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q3"), '0',
                            AURA_STRING_LITERAL("q3"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q3"), '1',
                            AURA_STRING_LITERAL("q3"));

  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("1000"));
  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("000001111"));
  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("0101"));

  aura_DFA_Machine_destroy(dfa_machine);

  return 0;
}
