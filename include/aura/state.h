#ifndef AURA_STATE_H_
#define AURA_STATE_H_

#include "aura/string.h"

typedef enum {
  AURA_STATE_GENERAL = 0x1,
  AURA_STATE_INITIAL = 0x1 << 1,
  AURA_STATE_FINAL = 0x1 << 2
} aura_StateType;

typedef struct {
  aura_StateType type;
  aura_String_t label;
} aura_State_t;

aura_State_t *aura_state_create(aura_StateType type, const char *label);
void aura_state_print(aura_State_t *state);
void aura_state_set(aura_State_t *state, aura_StateType type);
void aura_state_destroy(aura_State_t *state);

#endif // AURA_STATE_H_