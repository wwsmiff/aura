#include "aura/string.h"
#include <stdio.h>
#include <stdlib.h>

aura_String_t aura_string_create() {
  aura_String_t string;
  string.capacity = 8;
  string.len = 0;
  string.data = (char *)(malloc(sizeof(char) * string.capacity));
  return string;
}

void aura_string_append(aura_String_t *string, char c) {
  if ((string->len + 1) >= string->capacity) {
    string->capacity *= 2;
    string->data =
        (char *)(realloc(string->data, sizeof(char) * string->capacity));
    string->data[string->len++] = c;
  } else {
    string->data[string->len++] = c;
  }
}

void aura_string_print(aura_String_t *string) {
  printf("aura_string(data: '");
  for (size_t i = 0; i < string->len; ++i) {
    printf("%c", string->data[i]);
  }
  printf("', len: %d)\n", string->len);
}

void aura_string_put(aura_String_t *string, const char *data) {
  size_t data_len = strlen(data);
  for (size_t i = 0; i < data_len; ++i) {
    aura_string_append(string, data[i]);
  }
}

void aura_string_clear(aura_String_t *string) {
  memset(string->data, 0, string->len * sizeof(char));
  string->len = 0;
}

void aura_string_destroy(aura_String_t *string) {
  free(string->data);
  string->len = 0;
}