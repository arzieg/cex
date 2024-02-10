#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funktion, die alle Übereinstimmungen mit einem regulären Ausdruck zurückgibt
void find_matches(const char *pattern, const char *text) {
  regex_t regex;
  int reti;
  regmatch_t pmatch[2];  // Array für Übereinstimmungen

  // Kompiliere den regulären Ausdruck
  reti = regcomp(&regex, pattern, 0);
  if (reti) {
    fprintf(stderr, "Could not compile regulare pattern\n");
    exit(1);
  }

  // Suche nach Übereinstimmungen im Text
  reti = regexec(&regex, text, 2, pmatch, 0) == 0;
  if (reti) {
    char *tmp;
    int n;
    printf("text = %s\n", text);
    // printf("pmatch[0] = %s\n", pmatch[0]);
    printf("Found: %.*s\n", (int)(pmatch[1].rm_eo - pmatch[1].rm_so),
           &text[pmatch[1].rm_so]);
    n = pmatch[1].rm_eo - pmatch[1].rm_so;
    tmp = (char *)malloc(n * sizeof(char));
    strcpy(tmp, &text[pmatch[1].rm_so]);
    printf("tmp = %s", tmp);
    printf("strlen = %ld", strlen(tmp));
    // printf("Found: %s\n", &text[pmatch[1].rm_so]);
    // text += pmatch[2].rm_eo;  // Gehe zum nächsten Teil des Texts
  }

  // Gib den Speicher frei
  regfree(&regex);
}

int readconf(char *filename) {
  FILE *fd;
  int nRet;
  size_t *t = malloc(0);
  char **gptr = malloc(sizeof(char *));
  *gptr = NULL;
  char pattern[120] = "\\bSU_NET_MACS1_DC2=\\((.*)\\)";
  // char pattern[120] = "\\bSU_NET_MACS1_DC2=\\(([^)]+)";

  // CustomString_check_regex(CustomString *target, char *customregex)

  if ((fd = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Could not open %s!\n", filename);
    return EXIT_FAILURE;
  }

  while ((nRet = getline(gptr, t, fd)) > 0) {
    find_matches(pattern, *gptr);
    // fputs(*gptr, stdout);
  }

  return EXIT_SUCCESS;
}
