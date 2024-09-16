#ifndef AURA_INTERPRETER_H_
#define AURA_INTERPRETER_H_

#include "aura/dfa.h"
#include "aura/string.h"

#define MAX_MACHINES 512

typedef enum { AURA_MACHINE_DFA } aura_MachineType;

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
};

/* clang-format off */


typedef union {
  aura_DFA_Machine_t *dfa;
} aura_Machine_Variant_t;

typedef struct {
  aura_MachineType type;
  aura_Machine_Variant_t variant;
} aura_Machine_t;

typedef struct {
  aura_TokenType type;
  aura_String_t value;
} aura_Token_t;

typedef struct {
  aura_Token_t *data;
  size_t size;
} aura_TokenCollection_t;

typedef struct {
  aura_String_t line;
  char current;
  size_t line_number;
  aura_Machine_t *machines[MAX_MACHINES];
  aura_Machine_t *current_machine;
} aura_Interpreter_t;

aura_Interpreter_t *aura_interpreter_create();
void aura_interpreter_run_line(aura_Interpreter_t *interpreter,
                               const char *data);
void aura_interpreter_destroy(aura_Interpreter_t *interpreter);

#endif // AURA_INTERPRETER_H_