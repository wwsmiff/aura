#ifndef AURA_MACHINE_H_
#define AURA_MACHINE_H_

#include "aura/dfa.h"

typedef enum { AURA_MACHINE_DFA } aura_MachineType;

typedef union {
  aura_DFA_Machine_t *dfa;
} aura_Machine_Variant_t;

typedef struct {
  aura_MachineType type;
  aura_Machine_Variant_t variant;
} aura_Machine_t;

#endif // AURA_MACHINE_H_
