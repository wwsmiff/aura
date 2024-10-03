#include "aura/state.h"
#include "aura/string.h"
#include <stdio.h>
#include <stdlib.h>

aura_State_t *aura_state_create(aura_StateType type, const char *label) {
  aura_State_t *state = (aura_State_t *)(malloc(sizeof(aura_State_t)));
  state->label = aura_string_create();
  state->type = type;
  aura_string_put(&state->label, label);
  return state;
}

void aura_state_print(aura_State_t *state) {
  printf("aura_state(label: '");
  for (size_t i = 0; i < state->label.len; ++i) {
    printf("%c", state->label.data[i]);
  }
  printf("', type: {");
  if (state->type & AURA_STATE_GENERAL && state->type & AURA_STATE_FINAL &&
      state->type & AURA_STATE_INITIAL) {
    printf("general, initial, final");
  }

  else if (state->type & AURA_STATE_GENERAL) {
    printf("general");
    if (state->type & AURA_STATE_INITIAL) {
      printf(", initial");
    }
    if (state->type & AURA_STATE_FINAL) {
      printf(", final");
    }
  }

  printf("})\n");
}

void aura_state_set_type(aura_State_t *state, aura_StateType type) {
  state->type |= type;
}

void aura_state_destroy(aura_State_t *state) {
  aura_string_destroy(&state->label);
  free(state);
}
