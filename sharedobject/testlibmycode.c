#include <stdio.h>
#include <stdlib.h>

#include "mycode.h"

int main(int argc, char **argv) {
  if (argc > 0) {
    printf("\n%s\n", argv[1]);
    printf("%s\n", reverse(argv[1]));
  }
  return EXIT_SUCCESS;
}