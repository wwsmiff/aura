#include "aura/pda.h"
#include "aura/error.h"
#include <stdlib.h>

aura_PDA_Machine_t *aura_PDA_Machine_create() {
  aura_PDA_Machine_t *machine =
      (aura_PDA_Machine_t *)(malloc(sizeof(aura_PDA_Machine_t)));

  for (size_t i = 0; i < MAX_STATES; ++i) {
    machine->states[i] = NULL;
  }

  machine->current_state = NULL;
  machine->input = aura_string_create();
  machine->stack = aura_stack_create();
  machine->actions = NULL;
  machine->action_len = 0;
  machine->active_states = 0;
  return machine;
}

void aura_PDA_Machine_destroy(aura_PDA_Machine_t *machine) {
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      aura_state_destroy(machine->states[i]);
      machine->states[i] = NULL;
    }
  }
  aura_string_destroy(&machine->input);
  aura_stack_destroy(&machine->stack);
  free(machine->actions);
  free(machine);
}

void aura_PDA_Machine_set_input(aura_PDA_Machine_t *machine,
                                const char *input) {
  aura_string_put(&machine->input, input);
}

void aura_PDA_Machine_add_state(aura_PDA_Machine_t *machine,
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

aura_State_t *aura_PDA_Machine_get_state(aura_PDA_Machine_t *machine,
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

void aura_PDA_Machine_set_action(aura_PDA_Machine_t *machine, const char *src,
                                 char trigger, char stack_top, const char *dest,
                                 aura_PDAOperationType op) {
  if (machine->action_len != machine->input.len * machine->active_states) {
    machine->action_len = (machine->input.len * machine->input.len) *
                          (machine->active_states * machine->active_states);
    machine->actions = (aura_PDA_Action_t *)(realloc(
        machine->actions, sizeof(aura_PDA_Action_t) * machine->action_len));
  }
  int hash = 0;
  int len = strlen(src);
  for (size_t i = 0; i < len; ++i) {
    hash += (int)(src[i]) * (i + 1);
  }
  hash += (int)(trigger);
  hash += (int)(stack_top);
  hash %= machine->action_len;

  aura_State_t *dest_state = aura_PDA_Machine_get_state(machine, dest);

  if (dest == NULL) {
    AURA_RUNTIME_ERROR("Unknown state '%s'.\n", dest);
  } else {
    machine->actions[hash] = (aura_PDA_Action_t){
        .stack_top = stack_top, .dest = dest_state, .op = op};
  }
}

aura_PDA_Action_t aura_PDA_Machine_get_action(aura_PDA_Machine_t *machine,
                                              const char *src, char trigger,
                                              char stack_top) {
  int hash = 0;
  int len = strlen(src);
  for (size_t i = 0; i < len; ++i) {
    hash += (int)(src[i]) * (i + 1);
  }
  hash += (int)(trigger);
  hash += (int)(stack_top);
  hash %= machine->action_len;
  return machine->actions[hash];
}

void aura_PDA_Machine_run(aura_PDA_Machine_t *machine, const char *input) {
  aura_stack_clear(&machine->stack);
  for (size_t i = 0; i < MAX_STATES; ++i) {
    if (machine->states[i] != NULL) {
      if (machine->states[i]->type & AURA_STATE_INITIAL) {
        machine->current_state = machine->states[i];
      }
    }
  }

  if (machine->current_state == NULL) {
    AURA_RUNTIME_ERROR("No initial state provided.\n");
  }

  size_t len = strlen(input);
  char input_mismatch = '\0';

  for (size_t i = 0; i < len; ++i) {
    if (aura_string_in(&machine->input, input[i]) == false) {
      input_mismatch = input[i];
      break;
    }
  }

  if (input_mismatch != '\0') {
    AURA_RUNTIME_ERROR("'%c' is not a part of the input set.\n",
                       input_mismatch);
  }

  for (int i = 0; i < len; ++i) {
    const char *src = machine->current_state->label.data;
    aura_PDA_Action_t action = aura_PDA_Machine_get_action(
        machine, src, input[i], aura_stack_top(&machine->stack));
    if (action.dest != NULL) {
      machine->current_state = action.dest;
      switch (action.op) {
      case AURA_PDA_NO_OPERATION:
        break;
      case AURA_PDA_PUSH:
        aura_stack_push(&machine->stack, input[i]);
        break;
      case AURA_PDA_POP:
        aura_stack_pop(&machine->stack);
        break;
      }
    }

    if (action.dest == NULL) {
      AURA_RUNTIME_ERROR("Undefined path for state '%s' input '%c' with top of "
                         "the stack '%c'.\n",
                         machine->current_state->label.data, input[i],
                         aura_stack_top(&machine->stack));
    }
  }

  if (machine->current_state->type & AURA_STATE_FINAL ||
      aura_stack_is_empty(&machine->stack)) {
    printf("test case: '%s', status: INPUT ACCEPETED\n", input);
  } else {
    printf("test case: '%s', status: INPUT NOT ACCEPETED\n", input);
  }
}
