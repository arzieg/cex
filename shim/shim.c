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

// run
// LD_PRELOAD=./shim.so ./randtest
// output
//   83
//   86
//  ....
int rand() {
  int (*original_rand)(void) = dlsym(RTLD_NEXT, "rand");
  return original_rand() % 100;
}

/* Debugging shim with gdb

    gdb ./randtest

    in gdb
      set environment LD_PRELOAD=./shim.so
      break rand()


*/