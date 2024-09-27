#include "aura/interpreter.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 2048

#define ASSERT(x, m)                                                           \
  do {                                                                         \
    if (!(x)) {                                                                \
      fprintf(stderr, "Assertion Error (%s): %s\n", #x, m);                    \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

int aura_run_file(const char *path) {
  aura_Interpreter_t *interpreter = NULL;
  FILE *source_file = fopen(path, "r");
  if (source_file == NULL) {
    fprintf(stderr, "Failed to open '%s'.\n", path);
    return 1;
  } else {
    interpreter = aura_interpreter_create();
    char line[BUFFER_SIZE] = {0};
    while (fgets(line, BUFFER_SIZE, source_file) != NULL) {
      aura_interpreter_run_line(interpreter, line);
    }
    fclose(source_file);
    aura_interpreter_destroy(interpreter);
  }

  return 0;
}

int main(int argc, char **argv) {
  argc--;
  argv++;

  if (strcmp(argv[0], "run") == 0) {
    if (argc < 2) {
      fprintf(stderr, "Expected file to run.\n");
      return 1;
    }
    int err = aura_run_file(argv[1]);
    if (err != 0) {
      return err;
    }
  } else if (strcmp(argv[0], "plot") == 0) {
    fprintf(stderr, "Plotting is unimplemented.\n");
    return 1;
  } else if (strcmp(argv[0], "help") == 0) {
    fprintf(stderr, "");
    return 1;
  } else {
    fprintf(stderr, "Unknown argument: '%s'.\n", argv[0]);
  }

  return 0;
}
