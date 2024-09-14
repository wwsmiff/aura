#ifndef AURA_STRING_H_
#define AURA_STRING_H_

#include <stdint.h>
#include <string.h>

typedef struct {
  char *data;
  size_t len;
  size_t capacity;
} aura_String_t;

aura_String_t aura_string_create();
void aura_string_destroy(aura_String_t *string);

void aura_string_append(aura_String_t *string, char c);
void aura_string_print(aura_String_t *string);
void aura_string_put(aura_String_t *string, const char *data);
void aura_string_clear(aura_String_t *string);

#endif // AURA_STRING_H_