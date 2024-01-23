#include <regex.h>
#include <stdio.h>

int main() {
  regex_t regex;
  int reti;
  char msgbuf[100];

  static const char *const string = "123pol45rttts7";
  static const char *const pattern = "[0-9]{3}";

  printf("\n %s", pattern);
  /* Kompilieren des Regex-Musters */
  reti = regcomp(&regex, pattern, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Konnte Regex-Muster nicht kompilieren\n");
    return 1;
  }

  /* Ausführen des Regex-Musters */
  reti = regexec(&regex, string, 0, NULL, 0);
  if (!reti) {
    printf("Regex-Muster gefunden\n");
  } else if (reti == REG_NOMATCH) {
    printf("Regex-Muster nicht gefunden\n");
  } else {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "Regex-Fehler: %s\n", msgbuf);
    return 1;
  }

  /* Freigeben des Regex-Musters */
  regfree(&regex);

  return 0;
}
