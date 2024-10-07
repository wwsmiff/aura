#include "aura/interpreter.h"
#include "aura/set.h"
#include "aura/token.h"
#include <stdio.h>
#include <stdlib.h>

#define AURA_INTERPRETER_ERROR(interpreter, n, ...)                            \
  do {                                                                         \
    aura_Token_t token =                                                       \
        interpreter->tokens.data[interpreter->token_index + n];                \
    interpreter->source.data[interpreter->current_token.line - 1]              \
        ->data[interpreter->source.data[interpreter->current_token.line - 1]   \
                   ->len] = '\0';                                              \
    aura_token_print(&interpreter->current_token);                             \
    char line_start[100];                                                      \
    fprintf(                                                                   \
        stderr, "%lld | %s\n", interpreter->current_token.line,                \
        interpreter->source.data[interpreter->current_token.line - 1]->data);  \
    sprintf(line_start, "%lld | \n", interpreter->current_token.line);         \
    for (size_t i = 0; i < token.start + strlen(line_start) - 2; ++i) {        \
      fprintf(stderr, " ");                                                    \
    }                                                                          \
    fprintf(stderr, "^\n");                                                    \
    fprintf(stderr, "Aura Compiletime Error: ");                               \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  } while (0);

aura_Interpreter_t *aura_interpreter_create() {
  aura_Interpreter_t *interpreter =
      (aura_Interpreter_t *)(malloc(sizeof(aura_Interpreter_t)));
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    interpreter->machines[i] = NULL;
  }
  interpreter->current_machine = NULL;
  interpreter->token_index = 0;

  interpreter->source = aura_string_set_create();

  return interpreter;
}

aura_Token_t aura_interpreter_peek(aura_Interpreter_t *interpreter,
                                   size_t offset) {
  if (interpreter->token_index + offset < interpreter->tokens.size) {
    return interpreter->tokens.data[interpreter->token_index + offset];
  } else {
    return (aura_Token_t){.type = AURA_TOKEN_NONE};
  }
}

void aura_interpreter_consume(aura_Interpreter_t *interpreter, size_t offset) {
  interpreter->current_token = aura_interpreter_peek(interpreter, offset);
  if (interpreter->current_token.type != AURA_TOKEN_NONE) {
    interpreter->token_index += offset;
  }
}

// void aura_interpreter_eat(aura_Interpreter_t *interpreter,
// aura_TokenType type) {
// if (aura_interpreter_peek(interpreter, 1).type == type) {
// aura_interpreter_consume(interpreter, 1);
// } else {
// AURA_INTERPRETER_ERROR(
// interpreter, "Expected token type '%s' but got '%s'.\n",
// aura_token_str_table[type],
// aura_token_str_table[aura_interpreter_peek(interpreter, 1).type]);
// }
// }

void aura_interpreter_eat(aura_Interpreter_t *interpreter, aura_TokenType type,
                          const char *message) {
  if (aura_interpreter_peek(interpreter, 1).type == type) {
    aura_interpreter_consume(interpreter, 1);
  } else {
    aura_token_print(&interpreter->current_token);
    AURA_INTERPRETER_ERROR(interpreter, 0, message);
  }
}

aura_String_Set_t aura_interpreter_parse_set(aura_Interpreter_t *interpreter) {
  aura_interpreter_consume(interpreter, 1);
  aura_String_Set_t set = aura_string_set_create();
  while (1) {
    if (interpreter->current_token.type == AURA_TOKEN_ID ||
        interpreter->current_token.type == AURA_TOKEN_STRING) {
      aura_string_set_append_string(&set, &interpreter->current_token.value);
      aura_interpreter_consume(interpreter, 1);
    } else if (interpreter->current_token.type == AURA_TOKEN_COMMA) {
      aura_interpreter_consume(interpreter, 1);
    } else if (interpreter->current_token.type == AURA_TOKEN_RBRACE) {
      break;
    } else if (interpreter->current_token.type == AURA_TOKEN_RPAREN ||
               interpreter->current_token.type == AURA_TOKEN_EOL ||
               interpreter->current_token.type == AURA_TOKEN_LBRACE) {
      AURA_INTERPRETER_ERROR(interpreter, 0, "Expected end of set.\n");
    }
  }
  return set;
}

void aura_interpreter_parse_constructor(aura_Interpreter_t *interpreter) {
  size_t argc = 0;
  while (1) {
    if (argc == 0) {
      aura_interpreter_eat(interpreter, AURA_TOKEN_LBRACE,
                           "Expected argument 1 to be a set.\n");
      aura_String_Set_t set = aura_interpreter_parse_set(interpreter);
      for (size_t i = 0; i < set.len; ++i) {
        set.data[i]->data[set.data[i]->len] = '\0';
        aura_DFA_Machine_add_state(interpreter->current_machine->variant.dfa,
                                   set.data[i]->data);
      }
      aura_string_set_destroy(&set);
    } else if (argc == 1) {
      aura_interpreter_eat(interpreter, AURA_TOKEN_LBRACE,
                           "Expected argument 2 to be a set.\n");
      aura_String_Set_t set = aura_interpreter_parse_set(interpreter);
      aura_String_t input = aura_string_create();
      for (size_t i = 0; i < set.len; ++i) {
        aura_string_append(&input, set.data[i]->data[0]);
      }
      input.data[input.len] = '\0';
      aura_DFA_Machine_set_input(interpreter->current_machine->variant.dfa,
                                 input.data);
      aura_string_destroy(&input);
      aura_string_set_destroy(&set);
    } else if (argc == 2) {
      aura_interpreter_eat(interpreter, AURA_TOKEN_LBRACE,
                           "Expected argument 3 to be a set.\n");
      aura_String_Set_t set = aura_interpreter_parse_set(interpreter);
      for (size_t i = 0; i < set.len; ++i) {
        set.data[i]->data[set.data[i]->len] = '\0';
        aura_State_t *state = aura_DFA_Machine_get_state(
            interpreter->current_machine->variant.dfa, set.data[i]->data);
        if (state == NULL) {
          AURA_INTERPRETER_ERROR(interpreter, 0, "Unknown state: '%s'.\n",
                                 set.data[i]->data);
        } else {
          aura_state_set_type(state, AURA_STATE_GENERAL | AURA_STATE_FINAL);
        }
      }
      aura_string_set_destroy(&set);
    } else if (argc == 3) {
      aura_interpreter_eat(interpreter, AURA_TOKEN_ID,
                           "Expected argument 4 to be a single state.\n");
      interpreter->current_token.value
          .data[interpreter->current_token.value.len] = '\0';

      aura_State_t *state =
          aura_DFA_Machine_get_state(interpreter->current_machine->variant.dfa,
                                     interpreter->current_token.value.data);
      if (state == NULL) {
        state->label.data[state->label.len] = '\0';
        AURA_INTERPRETER_ERROR(interpreter, 0, "Unknown state: '%s'.\n",
                               state->label.data);
      } else {
        aura_state_set_type(state, AURA_STATE_GENERAL | AURA_STATE_INITIAL);
      }
    }
    if (aura_interpreter_peek(interpreter, 1).type == AURA_TOKEN_RPAREN) {
      break;
    }
    aura_interpreter_eat(interpreter, AURA_TOKEN_COMMA,
                         "Expected comma or end of construtor.\n");
    argc++;
  }
}

void aura_construct_machine(aura_Interpreter_t *interpreter, aura_String_t id,
                            aura_MachineType type) {
  if (type == AURA_MACHINE_DFA) {
    int machine_hash = 0;
    for (size_t i = 0; i < id.len; ++i) {
      machine_hash += (int)(id.data[i]) * (i + 1);
    }
    machine_hash += id.len;
    machine_hash %= MAX_MACHINES;
    interpreter->machines[machine_hash] =
        (aura_Machine_t *)(malloc(sizeof(aura_Machine_t)));
    interpreter->current_machine = interpreter->machines[machine_hash];
    interpreter->current_machine->type = AURA_MACHINE_DFA;
    interpreter->current_machine->variant.dfa = aura_DFA_Machine_create();
    aura_interpreter_eat(interpreter, AURA_TOKEN_LPAREN,
                         "Expected beginning of constructor.\n");
    aura_interpreter_parse_constructor(interpreter);
  }
}

void aura_define_machine(aura_Interpreter_t *interpreter, aura_String_t id) {
  int machine_hash = 0;
  for (size_t i = 0; i < id.len; ++i) {
    machine_hash += (int)(id.data[i]) * (i + 1);
  }
  machine_hash += id.len;
  machine_hash %= MAX_MACHINES;

  interpreter->current_machine = interpreter->machines[machine_hash];

  if (interpreter->current_machine == NULL) {
    id.data[id.len] = '\0';
    AURA_INTERPRETER_ERROR(interpreter, 0, "Machine '%s' does not exist.\n",
                           id.data);
  } else {
    int depth = 1;
    do {
      aura_String_t state_label;
      aura_String_t dest;
      aura_String_Set_t set;
      do {
        if (interpreter->current_token.type == AURA_TOKEN_ID) {
          if (aura_interpreter_peek(interpreter, 1).type == AURA_TOKEN_COLON) {
            state_label = interpreter->current_token.value;
            aura_interpreter_consume(interpreter, 1);
            if (aura_interpreter_peek(interpreter, 1).type ==
                AURA_TOKEN_KEYWORD) {
              aura_interpreter_consume(interpreter, 1);
              if (aura_string_compare_sd(&interpreter->current_token.value,
                                         "loop") == 1) {
                state_label.data[state_label.len] = '\0';
                for (size_t i = 0;
                     i < interpreter->current_machine->variant.dfa->input.len;
                     ++i) {
                  aura_DFA_Machine_set_path(
                      interpreter->current_machine->variant.dfa,
                      state_label.data,
                      interpreter->current_machine->variant.dfa->input.data[i],
                      state_label.data);
                }
                aura_interpreter_consume(interpreter, 1);
              } else {
                AURA_INTERPRETER_ERROR(interpreter, 0, "Unexpected keyword.\n");
              }
            } else {
              aura_interpreter_eat(interpreter, AURA_TOKEN_LBRACE,
                                   "Expected expression.\n");
            }
          }
        } else if (interpreter->current_token.type == AURA_TOKEN_LBRACE) {
          depth++;
          set = aura_interpreter_parse_set(interpreter);
        } else if (interpreter->current_token.type == AURA_TOKEN_RBRACE) {
          depth--;
          aura_interpreter_consume(interpreter, 1);
        } else if (interpreter->current_token.type ==
                   AURA_TOKEN_PATH_CONSTRUCT) {
          aura_interpreter_eat(interpreter, AURA_TOKEN_ID, "Expected state.\n");
          dest = interpreter->current_token.value;
          aura_interpreter_consume(interpreter, 1);
          state_label.data[state_label.len] = '\0';
          dest.data[dest.len] = '\0';
          for (int i = 0; i < set.len; ++i) {
            aura_DFA_Machine_set_path(interpreter->current_machine->variant.dfa,
                                      state_label.data, set.data[i]->data[0],
                                      dest.data);
          }
        } else if (interpreter->current_token.type == AURA_TOKEN_COMMA) {
          aura_interpreter_consume(interpreter, 1);
        }
      } while (interpreter->current_token.type != AURA_TOKEN_EOL);
      aura_interpreter_consume(interpreter, 1);
      if (depth == 1) {
        if (interpreter->current_token.type == AURA_TOKEN_RBRACE ||
            interpreter->current_token.type == AURA_TOKEN_ID ||
            interpreter->current_token.type == AURA_TOKEN_EOL) {
          if (interpreter->current_token.type == AURA_TOKEN_RBRACE) {
            depth--;
            aura_interpreter_consume(interpreter, 1);
          }
        } else {
          AURA_INTERPRETER_ERROR(interpreter, 0,
                                 "Expected end of machine definition.\n");
        }
      }
    } while (depth != 0);
  }
}

void aura_interpreter_run(aura_Interpreter_t *interpreter,
                          aura_String_t *string) {
  aura_String_t line = aura_string_create();

  for (size_t i = 0; i < string->len; ++i) {
    if (string->data[i] == '\n' || string->data[i] == '\r') {
      aura_string_set_append_string(&interpreter->source, &line);
      aura_string_clear(&line);
    } else {
      aura_string_append(&line, string->data[i]);
    }
  }

  aura_string_destroy(&line);

#define CURRENT_TOKEN (interpreter->current_token)
  interpreter->tokens = aura_tokenize_source(string);
  CURRENT_TOKEN = interpreter->tokens.data[0];
  while (CURRENT_TOKEN.type != AURA_TOKEN_NONE) {
    if (CURRENT_TOKEN.type == AURA_TOKEN_EOL) {
      aura_interpreter_consume(interpreter, 1);
    } else if (CURRENT_TOKEN.type == AURA_TOKEN_ID) {
      aura_String_t machine_id = CURRENT_TOKEN.value;
      aura_interpreter_consume(interpreter, 1);
      if (CURRENT_TOKEN.type == AURA_TOKEN_INITIALIZE) {
        aura_interpreter_eat(interpreter, AURA_TOKEN_KEYWORD,
                             "Unknown machine type.\n");
        aura_String_t machine_type = CURRENT_TOKEN.value;
        if (aura_string_compare_sd(&machine_type, "DFA")) {
          aura_construct_machine(interpreter, machine_id, AURA_MACHINE_DFA);
          aura_interpreter_eat(interpreter, AURA_TOKEN_RPAREN,
                               "Expected end of constructor.\n");
          aura_interpreter_eat(interpreter, AURA_TOKEN_EOL,
                               "Unexpected EOL.\n");
          aura_interpreter_consume(interpreter, 1);
        } else {
          machine_type.data[machine_type.len] = '\0';
          AURA_INTERPRETER_ERROR(interpreter, 0, "Unknown machine type '%s'",
                                 machine_type.data);
        }
      } else if (CURRENT_TOKEN.type == AURA_TOKEN_DEFINE) {
        aura_interpreter_eat(interpreter, AURA_TOKEN_LBRACE,
                             "Expected beginning of machine definition.\n");
        aura_interpreter_eat(interpreter, AURA_TOKEN_EOL, "Expected EOL.\n");
        aura_interpreter_consume(interpreter, 1);
        aura_define_machine(interpreter, machine_id);
        aura_interpreter_consume(interpreter, 1);
      } else {
        AURA_INTERPRETER_ERROR(interpreter, 0, "Unknown operator.\n");
      }
    } else if (CURRENT_TOKEN.type == AURA_TOKEN_KEYWORD) {
      if (aura_string_compare_sd(&CURRENT_TOKEN.value, "run")) {
        aura_interpreter_eat(interpreter, AURA_TOKEN_LPAREN,
                             "Expected open paranthesis.\n");
        aura_interpreter_eat(interpreter, AURA_TOKEN_ID,
                             "Expected machine name.\n");
        aura_String_t machine_id = interpreter->current_token.value;
        int machine_hash = 0;
        for (size_t i = 0; i < machine_id.len; ++i) {
          machine_hash += (int)(machine_id.data[i]) * (i + 1);
        }
        machine_hash += machine_id.len;
        machine_hash %= MAX_MACHINES;
        interpreter->current_machine = interpreter->machines[machine_hash];
        if (interpreter->current_machine == NULL) {
          machine_id.data[machine_id.len] = '\0';
          AURA_INTERPRETER_ERROR(interpreter, 0, "Unknown machine '%s'.",
                                 machine_id);
        } else {
          aura_interpreter_eat(interpreter, AURA_TOKEN_COMMA,
                               "Expected comma.\n");
          aura_interpreter_eat(interpreter, AURA_TOKEN_STRING,
                               "Expected string literal.\n");
          aura_String_t test_string = interpreter->current_token.value;
          test_string.data[test_string.len] = '\0';
          aura_DFA_Machine_run(interpreter->current_machine->variant.dfa,
                               test_string.data);
          aura_interpreter_eat(interpreter, AURA_TOKEN_RPAREN,
                               "Expected end of function call.\n");
        }
        aura_interpreter_eat(interpreter, AURA_TOKEN_EOL, "Unexpected EOL.\n");
        continue;
      }
    }
  }

#undef CURRENT_TOKEN
}

void aura_interpreter_destroy(aura_Interpreter_t *interpreter) {
  aura_token_array_destroy(&interpreter->tokens);
  aura_string_set_destroy(&interpreter->source);
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    if (interpreter->machines[i] != NULL) {
      if (interpreter->machines[i]->type == AURA_MACHINE_DFA) {
        aura_DFA_Machine_destroy(interpreter->machines[i]->variant.dfa);
      }
    }
  }
  free(interpreter);
}
