#ifndef AURA_INTERPRETER_H_
#define AURA_INTERPRETER_H_

#include "aura/dfa.h"
#include "aura/string.h"

#define MAX_MACHINES 512

typedef enum { AURA_MACHINE_DFA } aura_MachineType;

typedef struct {
  aura_MachineType type;
  typedef union {
    aura_DFA_Machine_t *dfa;
  } variant;
} aura_Machine_t;

typedef struct {
  aura_String_t line;
  char current;
  size_t line;
  aura_Machine_t *machines[MAX_MACHINES];
  aura_Machine_t *current_machine;
} aura_Interpreter_t;

aura_Interpreter_t aura_interpreter_create();
void aura_interpreter_readline(aura_Interpreter_t *interpreter,
                               const char *data);
void aura_interpreter_destroy(aura_Interpreter_t *interpreter);

#endif // AURA_INTERPRETER_H_