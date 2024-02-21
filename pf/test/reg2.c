#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiere das Muster für Werte zwischen doppelten Anführungszeichen
#define PATTERN "([^\"].*?)"

int main() {
  const char *input = "\"Foo Bar\" \"Another Value\" something else";

  regex_t regex;
  int ret = regcomp(&regex, PATTERN, REG_EXTENDED);
  if (ret != 0) {
    fprintf(stderr, "Fehler beim Kompilieren des regulären Ausdrucks\n");
    return 1;
  }

  regmatch_t matches[2];
  if (regexec(&regex, input, 2, matches, 0) == 0) {
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    char *extracted_value = strndup(input + start, end - start);
    printf("Extrahierter Wert: %s\n", extracted_value);
    free(extracted_value);
  } else {
    fprintf(stderr, "Kein passender Wert gefunden\n");
  }

  regfree(&regex);
  return 0;
}
