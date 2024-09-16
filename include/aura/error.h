#ifndef AURA_ERROR_H_
#define AURA_ERROR_H_

#include "aura/state.h"

#define AURA_REPORT_RUTIME_ERRORS 1
#define AURA_EXIT_ON_RUTIME_ERROR 1

#if AURA_EXIT_ON_RUTIME_ERROR == 1
#define AURA_EXIT(x) exit(x);
#else
#define AURA_EXIT(x)
#endif

#if AURA_REPORT_RUTIME_ERRORS == 1
#define AURA_RUNTIME_ERROR(...)                                                \
  do {                                                                         \
    fprintf(stderr, "Aura Runtime Error: ");                                   \
    fprintf(stderr, __VA_ARGS__);                                              \
    AURA_EXIT(1)                                                               \
  } while (0);

#else
#define AURA_RUNTIME_ERROR(...)
#endif

#define AURA_COMPILETIME_ERROR(...)                                            \
  do {                                                                         \
    fprintf(stderr, "Aura Compile Error: ");                                   \
    fprintf(stderr, __VA_ARGS__);                                              \
    exit(1);                                                                   \
  } while (0);

typedef enum {
  AURA_RUNTIME_ERROR_NONE,
  AURA_RUNTIME_ERROR_UNDEFINED_PATH,
  AURA_RUNTIME_ERROR_UNDEFINED_INPUT,
} aura_RuntimeErrorType;

#endif // AURA_ERROR_H_