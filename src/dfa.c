#include "aura/dfa.h"
#include "aura/error.h"
#include <stdbool.h>
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
                                aura_String_t input) {
  aura_string_copy(&machine->input, input);
}

void aura_DFA_Machine_add_state(aura_DFA_Machine_t *machine,
                                aura_String_t label) {
  int hash = 0;
  int len = label.len;
  for (size_t i = 0; i < len; ++i) {
    hash += (int)(label.data[i]) * (i + 1);
  }
  hash += len;
  hash %= MAX_STATES;
  machine->states[hash] = aura_state_create(AURA_STATE_GENERAL, label);
  machine->active_states++;
}

void aura_DFA_Machine_set_path(aura_DFA_Machine_t *machine, aura_String_t src,
                               char trigger, aura_String_t dest) {
  int prev_path_len = machine->path_len;
  if (machine->path_len != machine->input.len * machine->active_states * 2) {
    machine->path_len = machine->input.len * (machine->active_states) * 2;
    bool *null_mask = (bool *)(malloc(sizeof(bool) * machine->path_len));
    memset(null_mask, 0, machine->path_len * sizeof(bool));
    for (size_t i = 0; i < prev_path_len; ++i) {
      if (machine->paths[i] != NULL) {
        null_mask[i] = 1;
      }
    }
    machine->paths = (aura_Path_t **)(realloc(
        machine->paths, sizeof(aura_Path_t *) * machine->path_len));
    for (size_t i = 0; i < machine->path_len; ++i) {
      if (null_mask[i] == 0) {
        machine->paths[i] = NULL;
      }
    }

    free(null_mask);
  }

  int path_hash = 0;
  int len = src.len;
  for (int i = 0; i < len; ++i) {
    path_hash += (int)(src.data[i]) * (i + 1);
  }
  path_hash += (int)(trigger);
  path_hash %= (machine->path_len);
  if (aura_DFA_Machine_get_state(machine, dest) == NULL) {
    AURA_RUNTIME_ERROR("Unknown state '%s'.\n", dest);
  } else {
    if (machine->paths[path_hash] != NULL) {
      for (size_t i = path_hash; i < machine->path_len; ++i) {
        if (machine->paths[i] == NULL) {
          path_hash = i;
          break;
        }
      }
    }
    machine->paths[path_hash] = (aura_Path_t *)(malloc(sizeof(aura_Path_t)));
    machine->paths[path_hash]->src = aura_string_create();
    aura_string_copy(&machine->paths[path_hash]->src, src);
    machine->paths[path_hash]->trigger = trigger;
    machine->paths[path_hash]->dest = aura_DFA_Machine_get_state(machine, dest);
  }
}

aura_Path_t *aura_DFA_Machine_get_path(aura_DFA_Machine_t *machine,
                                       aura_String_t src, char trigger) {
  int path_hash = 0;
  int len = src.len;
  for (int i = 0; i < len; ++i) {
    path_hash += (int)(src.data[i]) * (i + 1);
  }
  path_hash += (int)(trigger);
  path_hash %= (machine->path_len);

  for (size_t i = path_hash; i < machine->path_len; ++i) {
    if (machine->paths[path_hash] != NULL &&
        aura_string_compare_ss(&machine->paths[path_hash]->src, &src)) {
      break;
    }
    path_hash = i;
  }

  return machine->paths[path_hash];
}

aura_State_t *aura_DFA_Machine_get_state(aura_DFA_Machine_t *machine,
                                         aura_String_t label) {
  int hash = 0;
  int len = label.len;
  for (size_t i = 0; i < len; ++i) {
    hash += (int)(label.data[i]) * (i + 1);
  }
  hash += len;
  hash %= MAX_STATES;
  return machine->states[hash];
}

aura_RuntimeErrorType aura_DFA_Machine_run(aura_DFA_Machine_t *machine,
                                           aura_String_t input) {
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      if (machine->states[i]->type & AURA_STATE_INITIAL) {
        machine->current_state = machine->states[i];
      }
    }
  }

  size_t len = input.len;
  char input_mismatch = '\0';

  for (size_t i = 0; i < len; ++i) {
    if (aura_string_in(&machine->input, input.data[i]) == false) {
      input_mismatch = input.data[i];
      break;
    }
  }

  if (input_mismatch != '\0') {
    AURA_RUNTIME_ERROR("'%c' is not a part of the input set.\n",
                       input_mismatch);
    return AURA_RUNTIME_ERROR_UNDEFINED_INPUT;
  }

  for (int i = 0; i < len; ++i) {
    aura_String_t src = machine->current_state->label;
    aura_Path_t *path = aura_DFA_Machine_get_path(machine, src, input.data[i]);

    if (path != NULL && path->dest != NULL) {
      machine->current_state = path->dest;
    } else {
      AURA_RUNTIME_ERROR("Undefined path for state '%s' input '%c'.\n",
                         machine->current_state->label.data, input.data[i]);
      return AURA_RUNTIME_ERROR_UNDEFINED_PATH;
    }
  }

  if (machine->current_state->type & AURA_STATE_FINAL) {
    printf("test case: '%.*s', status: INPUT ACCEPETED\n", input.len,
           input.data);
  } else {
    printf("test case: '%.*s', status: INPUT NOT ACCEPETED\n", input.len,
           input.data);
  }

  return AURA_RUNTIME_ERROR_NONE;
}

void aura_DFA_Machine_destroy(aura_DFA_Machine_t *machine) {
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      aura_state_destroy(machine->states[i]);
    }
  }

  for (size_t i = 0; i < machine->path_len; ++i) {
    if (machine->paths[i] != NULL) {
      aura_string_destroy(&machine->paths[i]->src);
      free(machine->paths[i]);
    }
  }

  aura_string_destroy(&machine->input);

  free(machine->paths);
  free(machine);
}
