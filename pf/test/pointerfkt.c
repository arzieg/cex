#include <stdio.h>

// Deklaration der Funktion mit Rückgabewert void *
void *myFunction(int a) {
  // Hier simulieren wir eine beliebige Speicheradresse
  int dummyValue = 42;
  void *address = &dummyValue;
  return address;
}

int main() {
  // Zeiger auf die Funktion
  void *(*functionPtr)() = &myFunction;

  // Rufe die Funktion auf und erhalte die Speicheradresse
  void *result = functionPtr();

  // Gib die Speicheradresse aus (als "reiner Speicherort")
  printf("Speicheradresse: %p\n", result);

  return 0;
}
