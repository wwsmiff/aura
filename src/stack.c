#include "aura/stack.h"
#include <stdio.h>
#include <stdlib.h>

aura_Stack_t aura_stack_create() {
  aura_Stack_t stack;
  stack.capacity = 8;
  stack.len = 0;
  stack.data = (char *)(malloc(sizeof(char) * stack.capacity));
  return stack;
}

void aura_stack_destroy(aura_Stack_t *stack) {
  free(stack->data);
  stack->data = NULL;
  stack->len = 0;
}

bool aura_stack_is_empty(aura_Stack_t *stack) { return stack->len == 0; }

void aura_stack_push(aura_Stack_t *stack, char c) {
  if ((stack->len + 1) >= stack->capacity) {
    stack->capacity *= 2;
    stack->data =
        (char *)(realloc(stack->data, sizeof(char) * stack->capacity));
  }
  stack->data[stack->len++] = c;
}

char aura_stack_pop(aura_Stack_t *stack) {
  char c = stack->data[stack->len - 1];
  stack->len--;
  return c;
}

char aura_stack_top(aura_Stack_t *stack) { return stack->data[stack->len - 1]; }

void aura_stack_print(aura_Stack_t *stack) {
  printf("aura_stack(\n");
  for (size_t i = 0; i < stack->len; ++i) {
    printf("\t%lld: %c", i, stack->data[i]);
    if (i == stack->len - 1) {
      printf(" <-\n");
    } else {
      printf("\n");
    }
  }
  printf(")\n");
}