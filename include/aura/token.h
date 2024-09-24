#ifndef AURA_TOKEN_H_
#define AURA_TOKEN_H_

#include "aura/string.h"
#include <stdint.h>

typedef enum {
  AURA_TOKEN_COMMENT,
  AURA_TOKEN_ID,
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
} aura_TokenType;

/* clang-format off */

static const char const *aura_token_str_table[] = {
  [AURA_TOKEN_COMMENT] = "token_comment",         
  [AURA_TOKEN_ID] = "token_id",         
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
};

/* clang-format off */

typedef struct {
  aura_TokenType type;
  aura_String_t value; 
} aura_Token_t;

typedef struct {
  aura_Token_t *data;
  size_t size;
} aura_TokenCollection_t;

void aura_token_print(aura_Token_t *token);
aura_TokenCollection_t aura_tokenize_line(aura_String_t *str);
void aura_token_collection_destroy(aura_TokenCollection_t *token_collection);


#endif // AURA_TOKEN_H_