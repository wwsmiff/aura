#include "aura/dfa.h"
#include "aura/error.h"
#include "aura/state.h"
#include "aura/string.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

#define ASSERT(x, m)                                                           \
  do {                                                                         \
    if (!(x)) {                                                                \
      fprintf(stderr, "Assertion Error (%s): %s\n", #x, m);                    \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

int main(int argc, char **argv) {
  // argc--;
  // argv++;

  // ASSERT(argc >= 1, "Expected filename.");

  aura_DFA_Machine_t *dfa_machine = aura_DFA_Machine_create();

  aura_DFA_Machine_add_state(dfa_machine, "q0");
  aura_DFA_Machine_add_state(dfa_machine, "q1");
  aura_DFA_Machine_add_state(dfa_machine, "q2");
  aura_DFA_Machine_set_input(dfa_machine, "ab");

  aura_state_set(aura_DFA_Machine_get_state(dfa_machine, "q0"),
                 AURA_STATE_INITIAL | AURA_STATE_GENERAL);
  aura_state_set(aura_DFA_Machine_get_state(dfa_machine, "q2"),
                 AURA_STATE_FINAL | AURA_STATE_GENERAL);

  aura_DFA_Machine_set_path(dfa_machine, "q0", 'a', "q1");
  aura_DFA_Machine_set_path(dfa_machine, "q1", 'a', "q1");
  aura_DFA_Machine_set_path(dfa_machine, "q1", 'b', "q2");
  aura_DFA_Machine_set_path(dfa_machine, "q2", 'a', "q0");
  aura_DFA_Machine_set_path(dfa_machine, "q2", 'b', "q2");

  aura_RuntimeError_t err = aura_DFA_Machine_run(dfa_machine, "abb");

  if (err.type == AURA_RUNTIME_ERROR_UNDEFINED_PATH) {
    AURA_RUNTIME_ERROR("Undefined path for state '%s' input '%c'.\n",
                       err.state->label.data, err.trigger);
    return 1;
  }

  aura_DFA_Machine_destroy(dfa_machine);

  return 0;
}
