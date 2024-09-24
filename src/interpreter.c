#include "aura/interpreter.h"
#include "aura/set.h"
#include "aura/token.h"
#include <stdio.h>
#include <stdlib.h>

aura_Interpreter_t *aura_interpreter_create() {
  aura_Interpreter_t *interpreter =
      (aura_Interpreter_t *)(malloc(sizeof(aura_Interpreter_t)));
  interpreter->line = aura_string_create();
  interpreter->line_number = 1;
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    interpreter->machines[i] = NULL;
  }
  interpreter->current_machine = NULL;

  return interpreter;
}

aura_String_Set_t aura_parse_set(aura_Interpreter_t *interpreter,
                                 aura_TokenCollection_t *tokens, size_t *idx) {
  aura_String_Set_t set = aura_string_set_create();
  while (1) {
    if (tokens->data[*idx].type == AURA_TOKEN_ID ||
        tokens->data[*idx].type == AURA_TOKEN_STRING) {
      aura_string_set_append_data(&set, tokens->data[*idx].value.data);
      (*idx)++;
      if (tokens->data[*idx].type == AURA_TOKEN_COMMA) {
        (*idx)++;
        continue;
      } else if (tokens->data[*idx].type == AURA_TOKEN_RBRACE) {
        (*idx)++;
        break;
      } else {
        AURA_COMPILETIME_ERROR("Expected comma or end of set, line: %lld.\n",
                               interpreter->line_number);
      }
    } else {
      AURA_COMPILETIME_ERROR("Expected comma or end of set, line: %lld.\n",
                             interpreter->line_number);
    }
  }
  return set;
}

void aura_construct_machine(aura_Interpreter_t *interpreter, const char *id,
                            aura_MachineType type,
                            aura_TokenCollection_t *tokens, size_t *idx) {
  int machine_hash = 0;
  int len = strlen(id);
  for (int i = 0; i < len; ++i) {
    machine_hash += (int)(id[i]) * (i + 1);
  }
  machine_hash += len;
  machine_hash %= MAX_MACHINES;
  if (type == AURA_MACHINE_DFA) {
    interpreter->machines[machine_hash] =
        (aura_Machine_t *)(malloc(sizeof(aura_Machine_t *)));
    interpreter->machines[machine_hash]->type = type;
    interpreter->machines[machine_hash]->variant.dfa =
        aura_DFA_Machine_create();
    aura_DFA_Machine_t *machine =
        interpreter->machines[machine_hash]->variant.dfa;
    (*idx)++;
    if ((*idx < tokens->size) && tokens->data[*idx].type == AURA_TOKEN_LPAREN) {
      (*idx)++;
      size_t argument = 0;
      aura_String_Set_t set;
      aura_String_t id = aura_string_create();
      while (1) {
        argument++;
        if ((*idx < tokens->size) &&
            tokens->data[*idx].type == AURA_TOKEN_LBRACE) {
          (*idx)++;
          set = aura_parse_set(interpreter, tokens, idx);
          if (tokens->data[*idx].type == AURA_TOKEN_COMMA) {
            (*idx)++;
          } else if (tokens->data[*idx].type == AURA_TOKEN_ID) {
            (*idx)++;
          }
        } else if ((*idx < tokens->size) &&
                   tokens->data[*idx].type == AURA_TOKEN_ID) {
          for (size_t i = 0; i < tokens->data[*idx].value.len; ++i) {
            aura_string_append(&id, tokens->data[*idx].value.data[i]);
          }
          (*idx)++;
        } else if ((*idx < tokens->size) &&
                   tokens->data[*idx].type == AURA_TOKEN_RPAREN) {
          break;
        } else {
          AURA_COMPILETIME_ERROR("Expected closing paranthesis, line: %lld\n",
                                 interpreter->line_number);
        }

        if (argument == 1) {
          if (set.len == 0) {
            AURA_COMPILETIME_ERROR("Argument %lld must be a set, line: %lld.\n",
                                   argument, interpreter->line_number);
          } else {
            for (size_t i = 0; i < set.len; ++i) {
              set.data[i]->data[set.data[i]->len] = '\0';
              aura_DFA_Machine_add_state(machine, set.data[i]->data);
            }
          }
        } else if (argument == 2) {
          if (set.len == 0) {
            AURA_COMPILETIME_ERROR("Argument %lld must be a set, line: %lld.\n",
                                   argument, interpreter->line_number);
          } else {
            aura_String_t input_str = aura_string_create();
            for (size_t i = 0; i < set.len; ++i) {
              if (set.data[i]->len > 1) {
                AURA_COMPILETIME_ERROR(
                    "Input must be a set of single characters, line: %lld.\n",
                    interpreter->line_number);
              }
              aura_string_append(&input_str, set.data[i]->data[0]);
            }
            aura_DFA_Machine_set_input(machine, input_str.data);
            aura_string_destroy(&input_str);
          }
        } else if (argument == 3) {
          if (set.len == 0) {
            AURA_COMPILETIME_ERROR("Argument %lld must be a set, line: %lld.\n",
                                   argument, interpreter->line_number);
          } else {
            for (size_t i = 0; i < set.len; ++i) {
              set.data[i]->data[set.data[i]->len] = '\0';
              aura_state_set(
                  aura_DFA_Machine_get_state(machine, set.data[i]->data),
                  AURA_STATE_GENERAL | AURA_STATE_FINAL);
            }
          }
        } else if (argument == 4) {
          if (id.len == 0) {
            AURA_COMPILETIME_ERROR(
                "Argument %lld must be a single state, line: %lld.\n", argument,
                interpreter->line_number);
          } else {
            id.data[id.len] = '\0';
            aura_state_set(aura_DFA_Machine_get_state(machine, id.data),
                           AURA_STATE_GENERAL | AURA_STATE_INITIAL);
          }
        }

        aura_string_clear(&id);
        aura_string_set_clear(&set);
      }
      aura_string_destroy(&id);
      aura_string_set_destroy(&set);
    }

    for (size_t i = 0; i < MAX_MACHINES; ++i) {
      if (machine->states[i] != NULL) {
        aura_state_print(machine->states[i]);
      }
    }
  }
}

void aura_interpreter_run_line(aura_Interpreter_t *interpreter,
                               const char *src) {
  aura_string_clear(&interpreter->line);
  aura_string_put(&interpreter->line, src);
  interpreter->line.len--;
  aura_TokenCollection_t tokens = aura_tokenize_line(&interpreter->line);
  for (size_t i = 0; i < tokens.size; ++i) {
    if (tokens.data[i].type == AURA_TOKEN_ID) {
      const char *machine_id = tokens.data[i].value.data;
      i++;
      if (tokens.data[i].type == AURA_TOKEN_INITIALIZE) {
        i++;
        if (tokens.data[i].type == AURA_TOKEN_ID) {
          aura_String_t machine_type_str =
              aura_string_create_and_put(tokens.data[i].value.data);
          aura_MachineType machine_type = 0;
          if (aura_string_compare_sd(&machine_type_str, "DFA")) {
            machine_type = AURA_MACHINE_DFA;
          } else {
            AURA_COMPILETIME_ERROR("Unknown machine type: '%s', line: %lld.\n",
                                   machine_type_str.data,
                                   interpreter->line_number);
          }
          aura_construct_machine(interpreter, machine_id, machine_type, &tokens,
                                 &i);
          aura_string_destroy(&machine_type_str);
        } else {
          AURA_COMPILETIME_ERROR("Expected machine type: '%s', line: %lld.\n",
                                 interpreter->line_number);
        }
      }
    }
  }
  interpreter->line_number++;
  aura_token_collection_destroy(&tokens);
}

void aura_interpreter_destroy(aura_Interpreter_t *interpreter) {
  aura_string_destroy(&interpreter->line);
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    aura_Machine_t *machine = interpreter->machines[i];
    if (machine != NULL) {
      if (machine->type == AURA_MACHINE_DFA) {
        aura_DFA_Machine_destroy(machine->variant.dfa);
      }
    }
  }
  free(interpreter);
}