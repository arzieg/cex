#include <stdio.h>
#include <string.h>

int main() {
  char *original;
  char *token, *theRest;

  original = strdup("192.168.65.0 192.168.65.1 24 665 1500 192.168.65.107");
  theRest = original;

  while ((token = strtok_r(theRest, " ", &theRest))) {
    printf("Token: %s\n", token);
  }
  return (0);
}