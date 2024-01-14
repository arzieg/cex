/* ===========================
   pf general helper functions
   =========================== */
#define _GNU_SOURCE  // to use getline, define before stdio.h
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

/* -------------------------------------------------
   CustomString - Hilfsfunktionen
   Eingabe von Werten und Rückgabe des Wertes
   -------------------------------------------------
*/
void CustomString_free(CustomString *target) {
  if (target->string) {
    free(target->string);
  }
  free(target);
}

bool CustomString_isalphanumeric(CustomString *target) {
  bool isalpha = true;
  for (int i = 0; i < target->length - 1; i++) {
    if (!isalnum(target->string[i])) {
      isalpha = false;
      break;
    }
  }
  return isalpha;
}

CustomString *custom_getline(FILE *stream, int minchars, int maxchars) {
  do {
    bool checklength = true;
    bool checkisalnum = false;
    CustomString *new = malloc(sizeof(*new));
    new->string = NULL;
    new->buffer_size = 0;
    new->length = getline(&(new->string), &(new->buffer_size), stream);
    // do some checks
    if ((new->length == -1) || (new->length > maxchars) ||
        (new->length < minchars)) {
      free(new);
      printf("\nExpect characters between %d-%d, try again >> ", minchars - 1,
             maxchars - 1);
      checklength = false;
    }
    if (CustomString_isalphanumeric(new)) {
      checkisalnum = true;
    } else {
      printf("Unvalid character found, valid characters are a-z,A-Z,0-9\n>> ");
    }
    if (checklength && checkisalnum) {
      return new;
    }
  } while (true);
}

bool get_yesno_status(char *text, FILE *stream) {
  bool answer = false;
  char character;
  printf(text);
  CustomString *new = malloc(sizeof(*new));
  new->string = NULL;
  new->buffer_size = 0;

  do {
    new->length = getline(&(new->string), &(new->buffer_size), stream);
    strncpy(&character, new->string, 1);
    character = toupper(character);
    answer = (character == 'Y') || (character == 'N');
    if (!answer) {
      printf("Please enter y or n >> ");
    }
  } while (!answer);

  CustomString_free(new);
  return (character == 'Y') ? true : false;
}

/*
toUpper - convert string to uppercase
*/
void toUpperCase(char *line, int n) {
  int count = 0;
  while (*line != '\0' && count <= n) {
    *line = toupper(*line);
    line++;
    count++;
  }
}
