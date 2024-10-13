#include "aura/pda.h"
#include "aura/stack.h"

int main() {
  aura_PDA_Machine_t *pda_machine = aura_PDA_Machine_create();
  aura_PDA_Machine_destroy(pda_machine);

  return 0;
}