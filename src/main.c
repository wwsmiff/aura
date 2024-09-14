#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 2048

#define ASSERT(x, m)                                                           \
  do {                                                                         \
    if (!(x)) {                                                                \
      fprintf(stderr, "Assertion Error (%s): %s\n", #x, m);                    \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

int main(int argc, char **argv) {
  argc--;
  argv++;

  return 0;
}