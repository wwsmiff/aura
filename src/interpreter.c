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
  interpreter->token_index = 0;

  return interpreter;
}

aura_Token_t aura_interpreter_peek(aura_Interpreter_t *interpreter,
                                   size_t offset) {
  if (interpreter->token_index + offset < interpreter->tokens.size) {
    return interpreter->tokens.data[interpreter->token_index];
  } else {
    return (aura_Token_t){.type = AURA_TOKEN_NONE};
  }
}

void aura_interpreter_consume(aura_Interpreter_t *interpreter, size_t offset) {
  interpreter->current_token = aura_interpreter_peek(interpreter, offset);
  if (interpreter->current_token.type != AURA_TOKEN_NONE) {
    interpreter->token_index += offset;
  }
}

void aura_interpreter_run(aura_Interpreter_t *interpreter,
                          aura_String_t *string) {
  interpreter->tokens = aura_tokenize_source(string);
  interpreter->current_token = interpreter->tokens.data[0];
  while (interpreter->current_token.type != AURA_TOKEN_NONE) {
    aura_token_print(&interpreter->current_token);
    aura_interpreter_consume(interpreter, 1);
  }
}

void aura_interpreter_destroy(aura_Interpreter_t *interpreter) {
  aura_token_array_destroy(&interpreter->tokens);
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
