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

size_t get_systeminformation_int(char *text, size_t min, size_t max) {
  size_t tmp;

  printf("\n%s, expected value between %ld and %ld: ", text, min, max);
  do {
    scanf("%ld", &tmp);
    if (tmp >= min && tmp <= max) break;
    printf("\nValue must between %ld and %ld, try again: ", min, max);
  } while (true);

  return tmp;
}

/*
  Split String in Single Token
  Used in readconf_su.c
*/
char **split_string(char *input, char delimiter) {
  char **tokens = NULL;
  char *token;
  char *rest = strdup(input);
  int count = 0;

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
  rest = input;
  int i = 0;

  // Fülle das Token-Array

  while ((token = strsep(&rest, &delimiter)) != NULL) {
    tokens[i] = strdup(token);
    i++;
  }
  tokens[i] = '\0';
  return tokens;
}
