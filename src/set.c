#include "aura/set.h"
#include "aura/string.h"
#include <stdio.h>
#include <stdlib.h>

aura_String_Set_t aura_string_set_create() {
  aura_String_Set_t set;
  set.capacity = 8;
  set.len = 0;
  set.data = (aura_String_t **)(malloc(sizeof(aura_String_t *) * set.capacity));
  return set;
}

void aura_string_set_destroy(aura_String_Set_t *set) {
  aura_string_set_clear(set);
  free(set->data);
}

void aura_string_set_print(aura_String_Set_t *set) {
  printf("aura_string_set(\n");
  for (size_t i = 0; i < set->len; ++i) {
    printf("\t");
    aura_string_print(set->data[i]);
  }
  printf(")\n");
}

void aura_string_set_append_string(aura_String_Set_t *set,
                                   aura_String_t *string) {
  if ((set->len + 1) >= set->capacity) {
    set->capacity *= 2;
    set->data = (aura_String_t **)(realloc(set->data, sizeof(aura_String_t **) *
                                                          set->capacity));
  }
  set->data[set->len++] = (aura_String_t *)(malloc(sizeof(aura_String_t)));
  *set->data[set->len - 1] = aura_string_create();
  for (size_t i = 0; i < string->len; ++i) {
    aura_string_append(set->data[set->len - 1], string->data[i]);
  }
}

void aura_string_set_append_data(aura_String_Set_t *set, const char *data) {
  if ((set->len + 1) >= set->capacity) {
    set->capacity *= 2;
    set->data = (aura_String_t **)(realloc(set->data, sizeof(aura_String_t **) *
                                                          set->capacity));
  }
  set->data[set->len++] = (aura_String_t *)(malloc(sizeof(aura_String_t)));
  *set->data[set->len - 1] = aura_string_create();
  aura_string_put(set->data[set->len - 1], data);
}

void aura_string_set_clear(aura_String_Set_t *set) {
  for (size_t i = 0; i < set->len; ++i) {
    aura_string_destroy(set->data[i]);
    free(set->data[i]);
    set->data[i] = NULL;
  }
  memset(set->data, 0, set->len * sizeof(aura_String_t *));
  set->len = 0;
}