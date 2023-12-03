#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 3) {
    printf(
        "\nexport LD_LIBRARY_PATH to the executable to find the shared "
        "library!\n");
    printf("usage: %s <library> <number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  char *librarypath = argv[1];
  int thenumber = atoi(argv[2]);

  void *libhandle = dlopen(librarypath, RTLD_LAZY);

  if (!libhandle) {
    fprintf(stderr, "dlopen error %s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  int (*fptr)(int);
  fptr = dlsym(libhandle, "do_operation");
  if (!fptr) {
    fprintf(stderr, "dlsym error %s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  char *(*fptr_libnamefunc)();
  fptr_libnamefunc = dlsym(libhandle, "get_library_name");
  if (!fptr_libnamefunc) {
    fprintf(stderr, "dlsym error %s\n", dlerror());
    exit(EXIT_FAILURE);
  }

  printf("Library: %s\n", fptr_libnamefunc());
  printf("%d --> %d\n", thenumber, fptr(thenumber));
  dlclose(libhandle);
}