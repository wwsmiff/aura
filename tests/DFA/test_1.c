/* Test to define a DFA which accepts
 * inputs {'a', 'b'} but is accepeted
 * only if the string ends with starts
 * with at least one 'a' and ends with 'b'.
 */

#include "aura/dfa.h"

int main() {
  aura_DFA_Machine_t *dfa_machine = aura_DFA_Machine_create();

  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q0"));
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q1"));
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q2"));
  aura_DFA_Machine_add_state(dfa_machine, AURA_STRING_LITERAL("q3"));

  aura_DFA_Machine_set_input(dfa_machine, AURA_STRING_LITERAL("ab"));

  aura_state_set_type(
      aura_DFA_Machine_get_state(dfa_machine, AURA_STRING_LITERAL("q0")),
      AURA_STATE_INITIAL | AURA_STATE_GENERAL);
  aura_state_set_type(
      aura_DFA_Machine_get_state(dfa_machine, AURA_STRING_LITERAL("q2")),
      AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q0"), 'a',
                            AURA_STRING_LITERAL("q1"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q0"), 'b',
                            AURA_STRING_LITERAL("q3"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q1"), 'a',
                            AURA_STRING_LITERAL("q1"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q1"), 'b',
                            AURA_STRING_LITERAL("q2"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q2"), 'a',
                            AURA_STRING_LITERAL("q0"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q2"), 'b',
                            AURA_STRING_LITERAL("q2"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q3"), 'a',
                            AURA_STRING_LITERAL("q3"));
  aura_DFA_Machine_set_path(dfa_machine, AURA_STRING_LITERAL("q3"), 'b',
                            AURA_STRING_LITERAL("q3"));

  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("aabaab"));
  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("baab"));
  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("baa"));
  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("caab"));
  aura_DFA_Machine_run(dfa_machine, AURA_STRING_LITERAL("ab"));

  aura_DFA_Machine_destroy(dfa_machine);

  return 0;
}
