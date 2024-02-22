#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definiere das Muster für Werte zwischen doppelten Anführungszeichen
// #define PATTERN "(?<=\")(.*)(?=\")"
#define PATTERN "\"(.*?)\"\1"
char *search = "SU_HANA_01_SAPREPO_VERSION_DC1=" PATTERN "";

int main() {
  const char *input =
      "SU_HANA_01_SAPREPO_VERSION_DC1=\"BWoH_Release_16\"\n \
       SU_HANA_01_SAPREPO_VERSION_DC2=\"BWoH_Release_17\"";

  regex_t regex;
  int ret = regcomp(&regex, search, REG_EXTENDED);
  if (ret != 0) {
    fprintf(stderr, "Fehler beim Kompilieren des regulären Ausdrucks\n");
    return 1;
  }

  regmatch_t matches[3];
  if (regexec(&regex, input, 3, matches, 0) == 0) {
    size_t start = matches[2].rm_so;
    size_t end = matches[2].rm_eo;
    char *extracted_value = strndup(input + start, end - start);
    printf("search: %s\n", search);
    printf("Extrahierter Wert: %s\n", extracted_value);
    free(extracted_value);
  } else {
    fprintf(stderr, "Kein passender Wert gefunden\n");
  }

  regfree(&regex);
  return 0;
}
