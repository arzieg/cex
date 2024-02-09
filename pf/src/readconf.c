#include <stdio.h>
#include <stdlib.h>

int readconf() {
  FILE *fd;
  char *datei = "testdatei.txt";
  int nRet;
  size_t *t = malloc(0);
  char **gptr = malloc(sizeof(char *));
  *gptr = NULL;

  if ((fd = fopen(datei, "r")) == NULL) {
    fprintf(stderr, "Konnte Datei %s nicht öffnen!\n", datei);
    return EXIT_FAILURE;
  }

  while ((nRet = getline(gptr, t, fd)) > 0) {
    fputs(*gptr, stdout);
  }

  return EXIT_SUCCESS;
}
