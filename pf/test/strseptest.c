#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funktion zum Aufteilen des Strings mit strsep und Rückgabe der Tokens in
// einem Array
char** split_string(char* input, char delimiter) {
  char** tokens = NULL;
  char* token;
  char* rest = strdup(input);
  int count = 0;

  // Zähle die Anzahl der Tokens
  while ((token = strsep(&rest, &delimiter)) != NULL) {
    count++;
  }

  // Alloziere Speicher für das Token-Array
  tokens = (char**)malloc(sizeof(char*) * count);
  if (!tokens) {
    perror("Fehler bei der Speicherzuweisung");
    exit(EXIT_FAILURE);
  }

  // Setze den Rest-Pointer zurück
  rest = input;
  int i = 0;

  // Fülle das Token-Array
  while ((token = strsep(&rest, &delimiter)) != NULL) {
    tokens[i] = strdup(token);
    i++;
  }

  return tokens;
}

int main() {
  char input[] = "Dies ist ein Beispiel mit Leerzeichen";
  printf("Eingabe: [%s]\n\n", input);

  char** token_array = split_string(input, ' ');

  if (token_array) {
    for (int i = 0; token_array[i] != NULL; i++) {
      printf("Wort %d: [%s]\n", i, token_array[i]);
      free(token_array[i]);
    }
    free(token_array);
  }

  return 0;
}
