#ifndef AURA_TOKEN_H_
#define AURA_TOKEN_H_

#include "aura/string.h"
#include <stdint.h>

#define KEYWORDS 4

typedef enum {
  AURA_TOKEN_NONE,
  AURA_TOKEN_COMMENT,
  AURA_TOKEN_ID,
  AURA_TOKEN_KEYWORD,
  AURA_TOKEN_LBRACE,
  AURA_TOKEN_RBRACE,
  AURA_TOKEN_LPAREN,
  AURA_TOKEN_RPAREN,
  AURA_TOKEN_DEFINE,
  AURA_TOKEN_INITIALIZE,
  AURA_TOKEN_PATH_CONSTRUCT,
  AURA_TOKEN_COMMA,
  AURA_TOKEN_QUOTE,
  AURA_TOKEN_STRING,
  AURA_TOKEN_COLON,
  AURA_TOKEN_EOL
} aura_TokenType;

/* clang-format off */

static const char *aura_token_str_table[] = {
  [AURA_TOKEN_NONE] = "token_none",
  [AURA_TOKEN_COMMENT] = "token_comment",         
  [AURA_TOKEN_ID] = "token_id",         
  [AURA_TOKEN_KEYWORD] = "token_keyword",
  [AURA_TOKEN_LBRACE] = "token_lbrace",
  [AURA_TOKEN_RBRACE] = "token_rbrace", 
  [AURA_TOKEN_LPAREN] = "token_lparen",
  [AURA_TOKEN_RPAREN] = "token_rparen",
  [AURA_TOKEN_DEFINE] = "token_define",
  [AURA_TOKEN_INITIALIZE] = "token_initialize",
  [AURA_TOKEN_PATH_CONSTRUCT] = "token_path_construct",
  [AURA_TOKEN_COMMA] = "token_comma",
  [AURA_TOKEN_QUOTE] = "token_quote",
  [AURA_TOKEN_STRING] = "token_string",
  [AURA_TOKEN_COLON] = "token_colon",
  [AURA_TOKEN_EOL] = "token_eol",
};


static const char *aura_reserved_keywords[] = {
  "run",
  "loop",
  "DFA",
  "plot"
};

/* clang-format off */

typedef struct {
  aura_TokenType type;
  aura_String_t value;
  size_t line;
  size_t start;
  size_t end;
} aura_Token_t;

typedef struct {
  aura_Token_t *data;
  size_t size;
} aura_Token_Array_t;

void aura_token_print(aura_Token_t *token);
aura_Token_Array_t aura_tokenize_source(aura_String_t *str);
void aura_token_array_destroy(aura_Token_Array_t *token_collection);


#endif // AURA_TOKEN_H_
