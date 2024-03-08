#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funktion zum Aufteilen des Strings mit strsep und Rückgabe der Tokens in
// einem Array
char** split_string(char* input, char delimiter) {
  char** tokens = NULL;
  char* token;
  char* rest = strdup(input);
  char mystring[20];
  int count;
  int i = 0;
  int n = 0;
  size_t maxlength = strlen(input);

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

  for (int j = 0; j < maxlength; j++) {
    if ((input[j] == delimiter) || (j == maxlength - 1)) {
      memcpy(mystring, input + n, j - n);
      mystring[j - n] = '\0';
      tokens[i] = strdup(mystring);
      printf("\nToken = %s\n", tokens[i]);
      n = j + 1;
      i++;
    }
  }

  return tokens;
}

int main() {
  char input[] = "192.168.65.0 192.168.65.1 24 665 1500 192.168.65.107";
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
