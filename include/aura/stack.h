#ifndef AURA_STACK_H_
#define AURA_STACK_H_

#include "aura/string.h"
#include <stdint.h>

typedef struct {
  char *data;
  size_t len;
  size_t capacity;
} aura_Stack_t;

aura_Stack_t aura_stack_create();
void aura_stack_destroy(aura_Stack_t *stack);
void aura_stack_push(aura_Stack_t *stack, char c);
bool aura_stack_is_empty(aura_Stack_t *stack);
char aura_stack_pop(aura_Stack_t *stack);
char aura_stack_top(aura_Stack_t *stack);
void aura_stack_print(aura_Stack_t *stack);

#endif // AURA_STACK_H_