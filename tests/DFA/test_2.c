/* Test to define a DFA with accepts inputs
 * {'0', '1'} with consecutive '0's and '1's and
 * starts with a '0'.
 */

#include "aura/dfa.h"

int main() {
  aura_DFA_Machine_t *dfa_machine = aura_DFA_Machine_create();
  aura_DFA_Machine_add_state(dfa_machine, "q0");
  aura_DFA_Machine_add_state(dfa_machine, "q1");
  aura_DFA_Machine_add_state(dfa_machine, "q2");
  aura_DFA_Machine_add_state(dfa_machine, "q3");

  aura_DFA_Machine_set_input(dfa_machine, "01");

  aura_state_set(aura_DFA_Machine_get_state(dfa_machine, "q0"),
                 AURA_STATE_INITIAL | AURA_STATE_GENERAL);

  aura_state_set(aura_DFA_Machine_get_state(dfa_machine, "q2"),
                 AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_DFA_Machine_set_path(dfa_machine, "q0", '0', "q1");
  aura_DFA_Machine_set_path(dfa_machine, "q0", '1', "q3");
  aura_DFA_Machine_set_path(dfa_machine, "q1", '1', "q2");
  aura_DFA_Machine_set_path(dfa_machine, "q1", '0', "q3");
  aura_DFA_Machine_set_path(dfa_machine, "q2", '0', "q1");
  aura_DFA_Machine_set_path(dfa_machine, "q2", '1', "q3");
  aura_DFA_Machine_set_path(dfa_machine, "q3", '0', "q3");
  aura_DFA_Machine_set_path(dfa_machine, "q3", '1', "q3");

  aura_DFA_Machine_run(dfa_machine, "1000");
  aura_DFA_Machine_run(dfa_machine, "000001111");
  aura_DFA_Machine_run(dfa_machine, "0101");

  return 0;
}