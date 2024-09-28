#include "aura/interpreter.h"
#include "aura/set.h"
#include "aura/token.h"
#include <stdio.h>
#include <stdlib.h>

aura_Interpreter_t *aura_interpreter_create() {
  aura_Interpreter_t *interpreter =
      (aura_Interpreter_t *)(malloc(sizeof(aura_Interpreter_t)));
  interpreter->line_number = 1;
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    interpreter->machines[i] = NULL;
  }
  interpreter->current_machine = NULL;
  interpreter->machine_definition = false;
  interpreter->brace_depth = 0;

  return interpreter;
}

void aura_interpreter_run(aura_Interpreter_t *interpreter,
                          aura_String_t *string) {
  aura_Token_Array_t tokens = aura_tokenize_source(string);
  for (size_t i = 0; i < tokens.size; ++i) {
    aura_token_print(&tokens.data[i]);
  }
  aura_token_array_destroy(&tokens);
}

void aura_interpreter_destroy(aura_Interpreter_t *interpreter) {
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
