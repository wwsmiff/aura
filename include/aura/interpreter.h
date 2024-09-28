#ifndef AURA_INTERPRETER_H_
#define AURA_INTERPRETER_H_

#include "aura/dfa.h"
#include "aura/string.h"
#include "aura/token.h"

#define MAX_MACHINES 512

typedef enum { AURA_MACHINE_DFA } aura_MachineType;

typedef union {
  aura_DFA_Machine_t *dfa;
} aura_Machine_Variant_t;

typedef struct {
  aura_MachineType type;
  aura_Machine_Variant_t variant;
} aura_Machine_t;

typedef struct {
  aura_Token_Array_t tokens;
  aura_Token_t current_token;
  size_t token_index;
  size_t line_number;
  aura_Machine_t *machines[MAX_MACHINES];
  aura_Machine_t *current_machine;
  bool machine_definition;
  size_t brace_depth;
} aura_Interpreter_t;

aura_Interpreter_t *aura_interpreter_create();
void aura_interpreter_run(aura_Interpreter_t *interpreter,
                          aura_String_t *string);
void aura_interpreter_destroy(aura_Interpreter_t *interpreter);

#endif // AURA_INTERPRETER_H_
