#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void split_string(char *input, char delimiter) {
  char **tokens = NULL;
  char *token = NULL;
  char *rest = strdup(input);
  // char *rest = strdup(input);
  // int count = 0;

  // Alloziere Speicher für das Token-Array
  tokens = (char **)malloc(sizeof(char *) * 4);
  if (!tokens) {
    perror("Fehler bei der Speicherzuweisung");
    exit(EXIT_FAILURE);
  }

  size_t i = 0;

  // Fülle das Token-Array

  while ((token = strsep(&rest, &delimiter)) != NULL) {
    tokens[i] = strdup(token);
    printf("\nToken = %s", tokens[i]);
    i++;
  }
  tokens[i] = '\0';

  i = 0;

  while (tokens[i] != NULL) {
    printf("\nFor Schleife token=%s", tokens[i]);
    i++;
  }
  free(tokens);
  tokens = NULL;

  // return tokens;
}

void block_memory(char *line) { split_string(line, ' '); }

int main() {
  char *line = "Das ist eine Zeile";
  block_memory(line);
}