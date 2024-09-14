/* Test to define a DFA which accepts
 * inputs {'a', 'b'} but is accepeted
 * only if the string ends with starts
 * with at least one 'a' and ends with 'b'.
 */

#include "aura/dfa.h"

int main() {
  aura_DFA_Machine_t *dfa_machine = aura_DFA_Machine_create();

  aura_DFA_Machine_add_state(dfa_machine, "q0");
  aura_DFA_Machine_add_state(dfa_machine, "q1");
  aura_DFA_Machine_add_state(dfa_machine, "q2");
  aura_DFA_Machine_add_state(dfa_machine, "q3");

  aura_DFA_Machine_set_input(dfa_machine, "ab");

  aura_state_set(aura_DFA_Machine_get_state(dfa_machine, "q0"),
                 AURA_STATE_INITIAL | AURA_STATE_GENERAL);
  aura_state_set(aura_DFA_Machine_get_state(dfa_machine, "q2"),
                 AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_DFA_Machine_set_path(dfa_machine, "q0", 'a', "q1");
  aura_DFA_Machine_set_path(dfa_machine, "q0", 'b', "q3");
  aura_DFA_Machine_set_path(dfa_machine, "q1", 'a', "q1");
  aura_DFA_Machine_set_path(dfa_machine, "q1", 'b', "q2");
  aura_DFA_Machine_set_path(dfa_machine, "q2", 'a', "q0");
  aura_DFA_Machine_set_path(dfa_machine, "q2", 'b', "q2");
  aura_DFA_Machine_set_path(dfa_machine, "q3", 'a', "q3");
  aura_DFA_Machine_set_path(dfa_machine, "q3", 'b', "q3");

  aura_DFA_Machine_run(dfa_machine, "aabaab");
  aura_DFA_Machine_run(dfa_machine, "baab");
  aura_DFA_Machine_run(dfa_machine, "baa");
  aura_DFA_Machine_run(dfa_machine, "caab");
  aura_DFA_Machine_run(dfa_machine, "ab");

  aura_DFA_Machine_destroy(dfa_machine);

  return 0;
}
