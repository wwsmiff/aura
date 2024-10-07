#include "aura/token.h"
#include "aura/error.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

aura_String_t aura_handle_id(aura_String_t *str, size_t *start) {
  aura_String_t id = aura_string_create();
  while (!isspace(str->data[*start]) && isalpha(str->data[*start]) &&
             (*start < str->len - 1) ||
         isdigit(str->data[*start]) || str->data[*start] == '_') {
    aura_string_append(&id, str->data[*start]);
    (*start)++;
  }
  (*start)--;
  return id;
}

aura_String_t aura_handle_string(aura_String_t *str, size_t *idx) {
  aura_String_t string_literal = aura_string_create();
  while (str->data[*idx] != '\'') {
    aura_string_append(&string_literal, str->data[*idx]);
    if (*idx >= str->len) {
      AURA_COMPILETIME_ERROR("Expected closing quote.\n");
    }
    (*idx)++;
  }
  return string_literal;
}

void aura_token_print(aura_Token_t *token) {
  printf("aura_token(type: ");
  printf("%s, value: '", aura_token_str_table[token->type]);
  for (size_t i = 0; i < token->value.len; ++i) {
    printf("%c", token->value.data[i]);
  }
  printf("', line: %lld, start: %lld, end: %lld", token->line, token->start,
         token->end);
  printf(")\n");
}

aura_Token_Array_t aura_tokenize_source(aura_String_t *str) {
  aura_Token_Array_t tokens;
  tokens.size = 0;
  tokens.data = (aura_Token_t *)(malloc(sizeof(aura_Token_t) * str->len));

  size_t line = 1;
  size_t line_idx = 0;

  for (size_t i = 0; i < str->len; ++i, ++line_idx) {
    if (str->data[i] == '\n' || str->data[i] == '\r') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_EOL,
                         .value = aura_string_create_and_put("\\n"),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx};
      line++;
      line_idx = 0;
    } else if (str->data[i] == '#') {
      // tokens.data[tokens.size++] = (aura_Token_t){
      //     .type = AURA_TOKEN_COMMENT, .value = aura_string_create()};
      while (str->data[i + 1] != '\n' && str->data[i + 1] != '\r') {
        // aura_string_append(&tokens.data[tokens.size - 1].value,
        // str->data[i]);
        i++;
        line_idx++;
      }
      continue;
    } else if (str->data[i] == '{') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_LBRACE,
                         .value = aura_string_create_and_put("{"),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx};

    } else if (str->data[i] == '}') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_RBRACE,
                         .value = aura_string_create_and_put("}"),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx};
    } else if (str->data[i] == '=') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_INITIALIZE,
                         .value = aura_string_create_and_put("="),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx};
    } else if (str->data[i] == ':') {
      if ((i + 1) < str->len && str->data[i + 1] == '=') {
        tokens.data[tokens.size++] =
            (aura_Token_t){.type = AURA_TOKEN_DEFINE,
                           .value = aura_string_create_and_put(":="),
                           .line = line,
                           .start = line_idx,
                           .end = line_idx + 1};
        i++;
        line_idx++;
        continue;
      } else {
        tokens.data[tokens.size++] =
            (aura_Token_t){.type = AURA_TOKEN_COLON,
                           .value = aura_string_create_and_put(":"),
                           .line = line,
                           .start = line_idx,
                           .end = line_idx + 1};
      }
    } else if (str->data[i] == '(') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_LPAREN,
                         .value = aura_string_create_and_put("("),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx + 1};
    } else if (str->data[i] == ')') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_RPAREN,
                         .value = aura_string_create_and_put(")"),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx + 1};
    } else if (isalpha(str->data[i])) {
      size_t start = i;
      aura_String_t id = aura_handle_id(str, &i);
      bool keyword = false;
      for (size_t j = 0; j < KEYWORDS; ++j) {
        if (aura_string_compare_sd(&id, aura_reserved_keywords[j]) == 1) {
          tokens.data[tokens.size++] =
              (aura_Token_t){.type = AURA_TOKEN_KEYWORD,
                             .value = id,
                             .line = line,
                             .start = line_idx,
                             .end = line_idx + (i - start)};
          line_idx += (i - start);
          keyword = true;
          break;
        }
      }
      if (!keyword) {
        tokens.data[tokens.size++] =
            (aura_Token_t){.type = AURA_TOKEN_ID,
                           .value = id,
                           .line = line,
                           .start = line_idx,
                           .end = line_idx + (i - start)};
        line_idx += (i - start);
      }
    } else if (str->data[i] == '-') {
      if ((i + 1) < str->len && str->data[i + 1] == '>') {
        tokens.data[tokens.size++] =
            (aura_Token_t){.type = AURA_TOKEN_PATH_CONSTRUCT,
                           .value = aura_string_create_and_put("->"),
                           .line = line,
                           .start = line_idx,
                           .end = line_idx + 1};
        i++;
        line_idx++;
        continue;
      } else {
        AURA_COMPILETIME_ERROR("Unknown token.\n");
      }
    } else if (str->data[i] == ',') {
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_COMMA,
                         .value = aura_string_create_and_put(","),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx};
    } else if (str->data[i] == '\'') {
      size_t start = i;
      i++;
      aura_String_t string_literal = aura_handle_string(str, &i);
      tokens.data[tokens.size++] =
          (aura_Token_t){.type = AURA_TOKEN_STRING,
                         .value = aura_string_create(),
                         .line = line,
                         .start = line_idx,
                         .end = line_idx + (i - start)};
      aura_Token_t *token = &tokens.data[tokens.size - 1];
      for (size_t i = 0; i < string_literal.len; ++i) {
        aura_string_append(&token->value, string_literal.data[i]);
      }
      aura_string_destroy(&string_literal);
      line_idx += (i - start);
    } else if (isspace(str->data[i])) {
      continue;
    } else {
      AURA_COMPILETIME_ERROR("Unexpected character: '%c'.\n", str->data[i]);
    }
  }
  return tokens;
}

void aura_token_array_destroy(aura_Token_Array_t *token_collection) {
  for (size_t i = 0; i < token_collection->size; ++i) {
    aura_string_destroy(&token_collection->data[i].value);
  }
  free(token_collection->data);
  token_collection->size = 0;
}
