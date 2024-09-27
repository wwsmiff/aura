#ifndef AURA_INTERPRETER_H_
#define AURA_INTERPRETER_H_

#include "aura/dfa.h"
#include "aura/string.h"

#define MAX_MACHINES 512
#define KEYWORDS 3

/* clang-format off */

static const char const *aura_reserved_keywords[] = {
  "run",
  "loop",
  "DFA"
};

/* clang-format on */

typedef enum { AURA_MACHINE_DFA } aura_MachineType;

typedef union {
  aura_DFA_Machine_t *dfa;
} aura_Machine_Variant_t;

typedef struct {
  aura_MachineType type;
  aura_Machine_Variant_t variant;
} aura_Machine_t;

typedef struct {
  aura_String_t line;
  size_t line_number;
  aura_Machine_t *machines[MAX_MACHINES];
  aura_Machine_t *current_machine;
  bool machine_definition;
  size_t brace_depth;
} aura_Interpreter_t;

aura_Interpreter_t *aura_interpreter_create();
void aura_interpreter_run_line(aura_Interpreter_t *interpreter,
                               const char *data);
void aura_interpreter_destroy(aura_Interpreter_t *interpreter);

#endif // AURA_INTERPRETER_H_
