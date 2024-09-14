#include "aura/dfa.h"
#include "aura/error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

aura_DFA_Machine_t *aura_DFA_Machine_create() {
  aura_DFA_Machine_t *machine =
      (aura_DFA_Machine_t *)(malloc(sizeof(aura_DFA_Machine_t)));
  for (size_t i = 0; i < MAX_STATES; ++i) {
    machine->states[i] = NULL;
  }
  machine->current_state = NULL;
  machine->input = aura_string_create();
  machine->active_states = 0;
  machine->path_len = 0;
  return machine;
}

void aura_DFA_Machine_set_input(aura_DFA_Machine_t *machine,
                                const char *input) {
  aura_string_put(&machine->input, input);
}

void aura_DFA_Machine_add_state(aura_DFA_Machine_t *machine,
                                const char *label) {
  int hash = 0;
  int len = strlen(label);
  for (size_t i = 0; i < len; ++i) {
    hash += (int)(label[i]) * (i + 1);
  }
  hash += len;
  hash %= MAX_STATES;
  machine->states[hash] = aura_state_create(AURA_STATE_GENERAL, label);
  machine->active_states++;
}

void aura_DFA_Machine_set_path(aura_DFA_Machine_t *machine, const char *src,
                               char trigger, const char *dest) {

  if (machine->path_len != machine->input.len * machine->active_states) {
    machine->path_len = machine->input.len * machine->active_states;
    machine->paths = (aura_State_t **)(realloc(
        machine->paths, sizeof(aura_State_t *) * machine->path_len));
  }

  int path_hash = 0;
  int len = strlen(src);
  for (int i = 0; i < len; ++i) {
    path_hash += (int)(src[i]) * (i + 1);
  }
  path_hash += (int)(trigger);
  path_hash %= (machine->path_len);
  machine->paths[path_hash] = aura_DFA_Machine_get_state(machine, dest);
}

aura_State_t *aura_DFA_Machine_get_path(aura_DFA_Machine_t *machine,
                                        const char *src, char trigger) {
  int path_hash = 0;
  int len = strlen(src);
  for (int i = 0; i < len; ++i) {
    path_hash += (int)(src[i]) * (i + 1);
  }
  path_hash += (int)(trigger);
  path_hash %= (machine->path_len);
  return machine->paths[path_hash];
}

aura_State_t *aura_DFA_Machine_get_state(aura_DFA_Machine_t *machine,
                                         const char *label) {
  int hash = 0;
  int len = strlen(label);
  for (size_t i = 0; i < len; ++i) {
    hash += (int)(label[i]) * (i + 1);
  }
  hash += len;
  hash %= MAX_STATES;
  return machine->states[hash];
}

aura_RuntimeError_t aura_DFA_Machine_run(aura_DFA_Machine_t *machine,
                                         const char *input) {
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      if (machine->states[i]->type & AURA_STATE_INITIAL) {
        machine->current_state = machine->states[i];
      }
    }
  }

  size_t len = strlen(input);

  for (int i = 0; i < len; ++i) {
    aura_State_t *next_state = aura_DFA_Machine_get_path(
        machine, machine->current_state->label.data, input[i]);
    if (next_state != NULL) {
      machine->current_state = next_state;
    }
    if (next_state == NULL) {
      return (aura_RuntimeError_t){.type = AURA_RUNTIME_ERROR_UNDEFINED_PATH,
                                   .state = machine->current_state,
                                   .trigger = input[i]};
    }
  }
  aura_state_print(machine->current_state);
  if (machine->current_state->type & AURA_STATE_FINAL) {
    printf("status: INPUT ACCEPETED\n");
  } else {
    printf("status: INPUT NOT ACCEPETED\n");
  }
  return (aura_RuntimeError_t){
      .type = AURA_RUNTIME_ERROR_NONE, .state = NULL, .trigger = '\0'};
}

void aura_DFA_Machine_destroy(aura_DFA_Machine_t *machine) {
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      aura_state_destroy(machine->states[i]);
      machine->states[i] = NULL;
    }
  }

  free(machine->paths);
  aura_string_destroy(&machine->input);
  free(machine);
}