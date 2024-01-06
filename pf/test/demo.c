// https://suchprogramming.com/c-strings-and-standard-input/
#define _GNU_SOURCE
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIDLENGTH 3

typedef struct {
  ssize_t length;
  char *string;
  size_t buffer_size;
} CustomString;

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

CustomString *custom_getline(FILE *stream, int maxchars) {
  do {
    bool checklength = true;
    bool checkisalnum = false;
    CustomString *new = malloc(sizeof(*new));
    new->string = NULL;
    new->buffer_size = 0;
    new->length = getline(&(new->string), &(new->buffer_size), stream);
    // do some checks
    if ((new->length == -1) || (new->length != maxchars)) {
      free(new);
      printf("\nExpect %d characters, try again >> ", maxchars - 1);
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

int main() {
  char text[2][SIDLENGTH + 1];
  bool mcos[2];
  char msg[255];

  for (int i = 0; i < 2; i++) {
    printf("\nPlease enter SID: ");
    CustomString *line = custom_getline(stdin, SIDLENGTH + 1);

    strncpy(text[i], line->string, line->length);
    // add \0 Terminator at end of SID
    text[i][line->length - 1] = '\0';
    // change to uppercase
    for (int j = 0; j < strnlen(text[i], SIDLENGTH); j++) {
      text[i][j] = toupper(text[i][j]);
    }

    printf("Read %zd bytes, buffer is %zd bytes\n", line->length,
           line->buffer_size);
    printf("Line read:%s\n", line->string);

    strncpy(msg,
            "is the SAP SID part of a MCOS System with multiples SIDs (Y/N)?",
            255);
    mcos[i] = get_yesno_status(msg, stdin);

    CustomString_free(line);
  }

  for (int i = 0; i < 2; i++) {
    printf("\nString %d = %s, mcos = %d", i, text[i], mcos[i]);
  }
  return 0;
}