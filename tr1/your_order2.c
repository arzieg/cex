#include <malloc.h>
#include <stdio.h>
#include <string.h>

char *order_words(char *ordered, const char *words) {
  // write to ordered and return it
  // the size of ordered is equal to 1 + strlen(words)
  char wordscpy[sizeof(words)];
  strcpy(wordscpy, words);
  char *rest = wordscpy;
  char *token;

  while ((token = strsep(&rest, " ")) != NULL) {
    printf("\nToken %s\n", token);

    for (int i = 1; i < 10; i++) {
      char tokencpy[sizeof(token)];
      strcpy(tokencpy, token);
      // const char *result = token;
      char *result = tokencpy;

      char target = (char)(i + '0');

      while ((result = strchr(result, target)) != NULL) {
        printf("\nFound '%c' starting at '%s'\n", target, result);
        ++result;  // Increment result, otherwise we'll find target at the same
                   // location
      }
      free(result);
    }
  }

  *ordered = '\0';
  return ordered;
}

int main(void) {
  char *text = "is2 Th1s T4est 3a";
  char *res = (char *)malloc(50 * sizeof(char));

  order_words(res, text);
  printf("\n%s ", res);

  return 0;
}