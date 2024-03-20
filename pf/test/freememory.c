#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXIT_FAILURE 1
#define FREE(ptr) \
  do {            \
    free(ptr);    \
    ptr = NULL;   \
  } while (0)

void split_line_and_assign_to_networktype(char *line);
char **split_string(char *input, char delimiter);

void split_line_and_assign_to_networktype(char *line) {
  printf("\nline in split_line_and_assign = %s", line);
  char **token_array = split_string(line, ' ');

  int cnt = 0;

  if (token_array) {
    for (int i = 0; token_array[i] != NULL; i++) {
      printf("\n Address = %p\n", token_array[i]);
      cnt++;
    }
  }
  // debug_print("\nCount=%d\n", cnt);

  if (token_array) {
    for (int i = 0; token_array[i] != NULL; i++) {
      printf("\nToken = %s", token_array[i]);

      FREE(token_array[i]);
    }
    FREE(token_array);
  }
}

char **split_string(char *input, char delimiter) {
  char **tokens = NULL;
  char *token = NULL;
  char *rest = strdup(input);
  int count = 0;

  printf("\nline in split-string = %s", input);

  // Zähle die Anzahl der Tokens
  while ((token = strsep(&rest, &delimiter)) != NULL) {
    count++;
  }

  // Alloziere Speicher für das Token-Array
  tokens = (char **)malloc(sizeof(char *) * count);
  if (!tokens) {
    perror("Fehler bei der Speicherzuweisung");
    exit(EXIT_FAILURE);
  }

  // Setze den Rest-Pointer zurück
  rest = strdup(input);
  int i = 0;

  // Fülle das Token-Array

  while ((token = strsep(&rest, &delimiter)) != NULL) {
    tokens[i] = strdup(token);
    i++;
  }
  tokens[i] = '\0';
  return tokens;
}

int main() {
  char *line = "Das ist ein Teststring";
  printf("\n Address of line %p", line);
  split_line_and_assign_to_networktype(line);

  return 0;
}