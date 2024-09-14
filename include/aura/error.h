#ifndef AURA_ERROR_H_
#define AURA_ERROR_H_

#include "aura/state.h"

#define AURA_RUNTIME_ERROR(...)                                                \
  do {                                                                         \
    fprintf(stderr, "Aura Runtime Error: ");                                   \
    fprintf(stderr, __VA_ARGS__);                                              \
  } while (0);

typedef enum {
  AURA_RUNTIME_ERROR_NONE,
  AURA_RUNTIME_ERROR_UNDEFINED_PATH,
} aura_RuntimeErrorType;

typedef struct {
  aura_RuntimeErrorType type;
  aura_State_t *state;
  char trigger;
} aura_RuntimeError_t;

#endif // AURA_ERROR_H_