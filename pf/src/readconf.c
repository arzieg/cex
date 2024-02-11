// implicit declaration of function getline
#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

/*
  ----------------------------------------
  search pattern in text and return result
  -----------------------------------------
*/
char *find_matches(const char *pattern, const char *text) {
  // https://gist.github.com/ianmackinnon/3294587

  size_t maxGroups = 3;
  size_t reti;

  regex_t regexCompiled;
  regmatch_t groupArray[maxGroups];
  char *cursor = NULL;

  if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
    printf("Could not compile regular expression.\n");
    return NULL;
  };

  reti = regexec(&regexCompiled, text, maxGroups, groupArray, 0);

  if (!reti) {
    regoff_t off, len;
    off = groupArray[1].rm_so;
    len = groupArray[1].rm_eo - groupArray[1].rm_so;
    cursor = malloc((len + 1) * sizeof(char));
    strncpy(cursor, text + off, len);
    cursor[len + 1] = '\0';
    debug_print("\n found = %s\n", cursor);
    }

  regfree(&regexCompiled);

  return (cursor != NULL) ? cursor : NULL;
}

int readconf(char *filename) {
  FILE *fd;
  int nRet;
  size_t *t = malloc(0);
  char **gptr = malloc(sizeof(char *));
  *gptr = NULL;
  char pattern[120] = "\\bSU_NET_MACS1_DC2=\\((.*)\\)";

  if ((fd = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Could not open %s!\n", filename);
    return EXIT_FAILURE;
  }

  while ((nRet = getline(gptr, t, fd)) > 0) {
    char *result = find_matches(pattern, *gptr);
    if (result != NULL) {
      printf("\nGot from function = %s \n", result);
    }
  }

  return EXIT_SUCCESS;
}
