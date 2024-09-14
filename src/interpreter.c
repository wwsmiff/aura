#include "aura/interpreter.h"

aura_Interpreter_t *aura_interpreter_create() {
  aura_Interpreter_t *interpreter =
      (aura_Interpreter_t *)(malloc(sizeof(aura_Interpreter_t)));
  interpreter->line = aura_string_create();
  interpreter->current = 0;
  interpreter->line = 0;
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    interpreter->machines[i] = NULL;
  }
  interpreter->current_machine = NULL;

  return interpreter;
}

void aura_interpreter_readline(aura_Interpreter_t *interpreter,
                               const char *data) {
  aura_string_clear(&interpreter->line);
  aura_string_put(&interpreter->line, data);
}

void aura_interpreter_destroy(aura_Interpreter_t *interpreter) {
  aura_string_destroy(&interpreter->line);
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    aura_Machine_t *machine = interpreter->machines[i];
    if (machine != NULL) {
      if (machine->type == AURA_MACHINE_DFA) {
        aura_DFA_Machine_destroy(machine->variant.dfa);
      }
    }
  }
  free(interpreter);
}