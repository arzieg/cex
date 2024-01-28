/* ===========================
   pf general helper functions
   =========================== */
#define _GNU_SOURCE  // to use getline, define before stdio.h
#include "pfhelper.h"

#include <ctype.h>
#include <errno.h>
#include <regex.h>
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

/* Check if string is alphanumeric*/
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

bool CustomString_check_custom_char(CustomString *target, char *customchar) {
  bool alphanum = true;
  bool flag = true;

  for (size_t i = 0; i < target->length - 1; i++) {
    flag = false;
    size_t n = sizeof(customchar) / sizeof(customchar[0]);
    for (size_t j = 0; j < n - 1; j++) {
      if (target->string[i] == customchar[j]) {
        flag = true;
        break;
      }
    }
    if (flag == false) {
      alphanum = false;
      break;
    }
  }

  return alphanum;
}

bool CustomString_check_regex(CustomString *target, char *customregex) {
  regex_t regex;
  regmatch_t pmatch[1];
  regoff_t len;
  int reti;
  char msgbuf[100];

  /* Kompilieren des Regex-Musters */
  reti = regcomp(&regex, customregex, REG_EXTENDED);
  if (reti != 0) {
    fprintf(stderr, "Could not compile regex.\n");
    return ERROR;
  }

  /* Ausführen des Regex-Musters */
  reti = regexec(&regex, target->string, 1, pmatch, 0);
  if (reti == 0) {
    debug_print("found regex reti=%d\n", reti);
  } else if (reti == REG_NOMATCH) {
    debug_print("regex not found reti = %d\n", reti);
  } else {
    regerror(reti, &regex, msgbuf, sizeof(msgbuf));
    fprintf(stderr, "regex-error: %s\n", msgbuf);
    return false;
  }

  /* Pruefung der Laenge des gefundenen Strings und vgl. mit Laenge des  */
  len = pmatch[0].rm_eo - pmatch[0].rm_so;
  debug_print("len pmatch[0] = %d,  target->length = %d\n", len,
              target->length);
  if (target->length - 1 - len != 0) {
    reti = 1;
    fprintf(stderr,
            "\nFound illegal character in the input string, please try again.");
  }

  /* Freigeben des Regex-Musters */
  regfree(&regex);

  return (reti == 0) ? true : false;
}

/* check if string is alphanumeric or has a punctuation*/
/* TODO: check if only one colon is in the string */
bool CustomString_isalpha_and_one_colon(CustomString *target) {
  bool isalphaandcolon = true;
  for (int i = 0; i < target->length - 1; i++) {
    if (!isalnum(target->string[i]) && !(target->string[i] == ':')) {
      isalphaandcolon = false;
      break;
    }
  }
  /* check if : is only once in the string */
  if (strchr(target->string, ':') != strrchr(target->string, ':'))
    isalphaandcolon = false;

  return isalphaandcolon;
}

bool CustomString_isalpha_or_colon(CustomString *target) {
  bool isalphaorcolon = true;
  for (int i = 0; i < target->length - 1; i++) {
    if (!isalnum(target->string[i]) && !(target->string[i] == ':')) {
      isalphaorcolon = false;
      break;
    }
  }

  return isalphaorcolon;
}

bool CustomString_isalpha_or_hyphen(CustomString *target) {
  bool isalphaorhyphen = true;
  for (int i = 0; i < target->length - 1; i++) {
    if (!isalnum(target->string[i]) && !(target->string[i] == '-')) {
      isalphaorhyphen = false;
      break;
    }
  }

  return isalphaorhyphen;
}

bool CustomString_isalpha_or_dot(CustomString *target) {
  bool isalphaordot = true;
  for (int i = 0; i < target->length - 1; i++) {
    if (!isalnum(target->string[i]) && !(target->string[i] == '.')) {
      isalphaordot = false;
      break;
    }
  }

  return isalphaordot;
}

/*
CustomString *custom_getline(FILE *stream, int minchars, int maxchars,
                             int stringfunction) {
  do {
    bool checklength = true;
    bool checkstring = false;
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

    switch (stringfunction) {
      case ISALPHANUMERIC:
        checkstring = CustomString_isalphanumeric(new);
        if (!checkstring)
          printf("Invalid character found, valid is a-z,A-Z,0-9\n >> ");
        break;
      case ISALPHA_AND_COLON:
        checkstring = CustomString_isalpha_and_one_colon(new);
        if (!checkstring)
          printf(
              "Invalid character found, valid is a-z,A-Z,0-9 and mandatory "
              "one "
              "colon : \n >> ");
        break;
      case ISAPLHA_OR_COLON:
        checkstring = CustomString_isalpha_or_colon(new);
        if (!checkstring)
          printf(
              "Invalid character found, valid is a-z,A-Z,0-9 and colon : \n "
              ">> ");
        break;
      case ISALPHA_OR_HYPHEN:
        checkstring = CustomString_isalpha_or_hyphen(new);
        if (!checkstring)
          printf(
              "Invalid character found, valid is a-z,A-Z,0-9 and hyphen - \n "
              ">> ");
        break;
      case ISALPHA_OR_DOT:
        checkstring = CustomString_isalpha_or_dot(new);
        if (!checkstring)
          printf(
              "Invalid character found, valid is a-z,A-Z,0-9 and dot . \n "
              ">> ");
        break;
      default:
        printf("Invalid character found. >> ");
    }

    if (checklength && checkstring) {
      return new;
    }
  } while (true);
}
*/

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
  to_upper_case - convert string to uppercase
*/
void to_upper_case(char *line, int n) {
  int count = 0;
  while (*line != '\0' && count <= n) {
    *line = toupper(*line);
    line++;
    count++;
  }
}

/*
  to_lower_case - convert string to lowercase
*/
void to_lower_case(char *line, int n) {
  size_t count = 0;
  while (*line != '\0' && count <= n) {
    *line = tolower(*line);
    line++;
    count++;
  }
}

/*
  clear stdin
  https://stackoverflow.com/questions/7898215/how-can-i-clear-an-input-buffer-in-c
*/
void clear_stdin(void) {
  while (true) {
    int c = getc(stdin);
    if (c == EOF || c == '\n') {
      break;
    }
  }
}

CustomString *custom_getline(FILE *stream, int minchars, int maxchars,
                             char *stringfunction) {
  do {
    bool checklength = true;
    bool checkstring = false;
    debug_print("Checkstring %d\n", checkstring);
    debug_print("Checklength %d\n", checklength);
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
    } else {
      /* change \n to \0 */
      new->string[new->length - 1] = '\0';
      checkstring = CustomString_check_regex(new, stringfunction);
      debug_print("Checkstring %d\n", checkstring);
      debug_print("Checklength %d\n", checklength);
    }

    if (checklength && checkstring) {
      return new;
    }
  } while (true);
}