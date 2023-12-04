#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// run
// LD_PRELOAD=./shim.so ./randtest
// output
//   42
//   42
//   ...

// int rand() { return 42; }
int rand() {
  int (*original_rand)(void) = dlsym(RTLD_NEXT, "rand");
  return original_rand() % 100;
}
