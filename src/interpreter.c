#include "aura/interpreter.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

aura_String_t aura_handle_id(aura_String_t *str, size_t *start) {
  aura_String_t id = aura_string_create();
  while (!isspace(str->data[*start]) && isalpha(str->data[*start]) &&
             (*start < str->len - 1) ||
         isdigit(str->data[*start])) {
    aura_string_append(&id, str->data[*start]);
    (*start)++;
  }
  (*start)--;
  return id;
}

void aura_token_print(aura_Token_t *token) {
  printf("aura_token(type: ");
  printf("%s, value: '", aura_token_str_table[token->type]);
  for (size_t i = 0; i < token->value.len; ++i) {
    printf("%c", token->value.data[i]);
  }
  printf("')\n");
}

aura_TokenCollection_t aura_tokenize_line(aura_String_t *str) {
  aura_TokenCollection_t tokens;
  tokens.size = 0;
  tokens.data = (aura_Token_t *)(malloc(sizeof(aura_Token_t) * str->len));
  for (size_t i = 0; i < str->len; ++i) {
    if (str->data[i] == '#') {
      tokens.data[tokens.size++] = (aura_Token_t){
          .type = AURA_TOKEN_COMMENT, .value = aura_string_create()};
      while (i < str->len) {
        aura_string_append(&tokens.data[tokens.size - 1].value, str->data[i]);
        i++;
      }
    } else if (str->data[i] == '{') {
      tokens.data[tokens.size++] = (aura_Token_t){
          .type = AURA_TOKEN_LBRACE, .value = aura_string_create_and_put("{")};
    } else if (str->data[i] == '}') {
      tokens.data[tokens.size++] = (aura_Token_t){
          .type = AURA_TOKEN_RBRACE, .value = aura_string_create_and_put("}")};
    } else if (str->data[i] == '=') {
      if ((i - 1) >= 0 && str->data[i - 1] == ':') {
        tokens.data[tokens.size++] =
            (aura_Token_t){.type = AURA_TOKEN_DEFINE,
                           .value = aura_string_create_and_put(":=")};
        continue;
      } else {
        tokens.data[tokens.size++] =
            (aura_Token_t){.type = AURA_TOKEN_INITIALIZE,
                           .value = aura_string_create_and_put("=")};
      }
    } else if (str->data[i] == '(') {
      tokens.data[tokens.size++] = (aura_Token_t){
          .type = AURA_TOKEN_LPAREN, .value = aura_string_create_and_put("(")};
    } else if (str->data[i] == ')') {
      tokens.data[tokens.size++] = (aura_Token_t){
          .type = AURA_TOKEN_RPAREN, .value = aura_string_create_and_put(")")};
    } else if (isalpha(str->data[i])) {
      aura_String_t id = aura_handle_id(str, &i);
      // printf("id: ");
      // aura_string_print(&id);
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_ID, .value = id};
    }
  }
  return tokens;
}

void aura_token_collection_destroy(aura_TokenCollection_t *token_collection) {
  for (size_t i = 0; i < token_collection->size; ++i) {
    aura_string_destroy(&token_collection->data[i].value);
  }
  free(token_collection->data);
  token_collection->size = 0;
}

aura_Interpreter_t *aura_interpreter_create() {
  aura_Interpreter_t *interpreter =
      (aura_Interpreter_t *)(malloc(sizeof(aura_Interpreter_t)));
  interpreter->line = aura_string_create();
  interpreter->current = 0;
  interpreter->line_number = 0;
  for (size_t i = 0; i < MAX_MACHINES; ++i) {
    interpreter->machines[i] = NULL;
  }
  interpreter->current_machine = NULL;

  return interpreter;
}

void aura_interpreter_run_line(aura_Interpreter_t *interpreter,
                               const char *src) {
  aura_string_clear(&interpreter->line);
  aura_string_put(&interpreter->line, src);
  interpreter->line.len--; // Omit '\n'
  aura_TokenCollection_t tokens = aura_tokenize_line(&interpreter->line);
  for (size_t i = 0; i < tokens.size; ++i) {
    aura_token_print(&tokens.data[i]);
  }
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