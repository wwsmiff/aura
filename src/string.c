#include "aura/string.h"
#include <stdbool.h>
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

void aura_string_copy(aura_String_t *dest, aura_String_t src) {
  // if (dest == NULL) {
  // *dest = aura_string_create();
  // }
  for (size_t i = 0; i < src.len; ++i) {
    aura_string_append(dest, src.data[i]);
  }
}

void aura_string_clear(aura_String_t *string) {
  memset(string->data, 0, string->len * sizeof(char));
  string->len = 0;
}

bool aura_string_in(aura_String_t *string, char c) {
  for (size_t i = 0; i < string->len; ++i) {
    if (string->data[i] == c) {
      return true;
    }
  }

  return false;
}

aura_String_t aura_string_create_and_put(const char *data) {
  aura_String_t string = aura_string_create();
  aura_string_put(&string, data);
  return string;
}

bool aura_string_compare_ss(aura_String_t *s1, aura_String_t *s2) {
  if (s1->len != s2->len) {
    return false;
  }

  // Try to add SIMD.
  for (size_t i = 0; i < s1->len; ++i) {
    if (s1->data[i] != s2->data[i]) {
      return false;
    }
  }
  return true;
}

bool aura_string_compare_sd(aura_String_t *s1, const char *data) {
  if (s1->len != strlen(data)) {
    return false;
  }
  for (size_t i = 0; i < s1->len; ++i) {
    if (s1->data[i] != data[i]) {
      return false;
    }
  }

  return true;
}

void aura_string_destroy(aura_String_t *string) {
  free(string->data);
  string->data = NULL;
  string->len = 0;
}
