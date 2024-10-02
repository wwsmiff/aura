#ifndef AURA_SET_H_
#define AURA_SET_H_

#include "aura/string.h"
#include <stdint.h>

typedef struct {
  aura_String_t **data;
  size_t len;
  size_t capacity;
} aura_String_Set_t;

aura_String_Set_t aura_string_set_create();
void aura_string_set_destroy(aura_String_Set_t *set);
void aura_string_set_print(aura_String_Set_t *set);
void aura_string_set_append_string(aura_String_Set_t *set,
                                   aura_String_t *string);
void aura_string_set_append_data(aura_String_Set_t *set, const char *data);
void aura_string_set_clear(aura_String_Set_t *set);

#endif // AURA_SET_H_
