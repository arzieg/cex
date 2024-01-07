#define _GNU_SOURCE  // to use getline, define before stdio.h
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

#define DEBUG 1

#define debug_print(fmt, ...)                                           \
  do {                                                                  \
    if (DEBUG)                                                          \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, \
              __VA_ARGS__);                                             \
  } while (0)

/* global variables */
/* each hanasystemenvironment has 1 to n servers */
HANASYSTEMTYPE hanasystem[MAX_ENVIRONMENTS][MAX_HOST_EACH_HANASYSTEM];
/* each hanasystemenvironment could have 1 to n SIDs */
SIDTYPE hanasid[MAX_ENVIRONMENTS][MAX_SID_PER_ENVIRONMENT];

/*
1. ScaleOut oder ScaleUp oder Managementserver (Toolserver, iSCSI Server)
   type=so,su,toolserver,iscsi
2. SID List
   sid[x], MCOS ja/nein
3. SystemReplikation / keine SystemReplikation
   sid[x], SR ja/nein
4. Im SO ist nur SingleSID erlaubt
5. MCOS nur im SU
*/

/* CustomString - Hilfsfunktionen
   Eingabe von Werten und Rückgabe des Wertes
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

int interactive(void) {
  // int selection;

  get_sid_list();

  // selection = get_systemtype_choice();
  // printf("\nYou entered %d\n", selection);

  return OK;
}

int get_systemtype_choice(void) {
  int selection = 0;

  do {
    system("clear");
    printf("\nInteractive Mode");
    printf("\n================\n\n");
    printf("\nWelcher Systemtyp soll definiert werden?");
    printf("\n1 - ScaleOut");
    printf("\n2 - ScaleUp");
    printf("\n3 - Toolserver");
    printf("\n4 - iSCSI Server");
    printf("\n5 - Quit");
    printf("\n>> ");
    scanf("%d", &selection);
  } while (selection < 1 || selection > 5);

  switch (selection) {
    case 5:
      printf("\nGood Bye\n");
      exit(0);
      break;

    default:
      break;
  }

  return selection;
}

/* --------------------------------------------------------------
 Enter the SID list. Each SID could be part of a systemReplication.
 If more then one System is entered, the system is per definition part of a mcos
 system
   -------------------------------------------------------------- */
void get_sid_list(void) {
  bool systemreplication;
  bool get_next_sid = false;
  bool next_loop = false;
  char msg[255];
  int n = 0;

  system("clear");
  printf("\nInteractive Mode");
  printf("\n================\n\n");

  do {
    printf("\nPlease enter SID: ");
    CustomString *line = custom_getline(stdin, SIDLENGTH + 1);

    strncpy(hanasid[0][n].sid, line->string, line->length);
    // add \0 Terminator at end of SID
    hanasid[0][n].sid[line->length - 1] = '\0';
    // change to uppercase
    for (int j = 0; j < strnlen(hanasid[0][n].sid, SIDLENGTH); j++) {
      hanasid[0][n].sid[j] = toupper(hanasid[0][n].sid[j]);
    }

    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line read:%s\n", line->string);

    strncpy(msg, "is the SAP SID part of a System Replication (Y/N)?", 255);
    systemreplication = get_yesno_status(msg, stdin);
    debug_print("... SR = %d\n", systemreplication);
    hanasid[0][n].systemReplication = systemreplication;

    n++;
    CustomString_free(line);

    strncpy(msg, "Enter an additional SAP SID (Y/N)?", 255);
    get_next_sid = get_yesno_status(msg, stdin);
    debug_print("... GET_NEXT_SID = %d\n", get_next_sid);

    if (n > MAX_SID_PER_ENVIRONMENT) {
      next_loop = false;
      fprintf(stderr, "Maximum SIDs reached, leave loop!");
    } else {
      next_loop = get_next_sid;
    }

  } while (next_loop);

  printf("\nYou entered:\n");
  n--;
  bool mcos;
  (n > 0) ? (mcos = true) : (mcos = false);

  for (int i = 0; i <= n; i++) {
    debug_print("SID %d, %s  MCOS=%d  SR=%d\n", i, hanasid[0][i].sid, mcos,
                hanasid[0][i].systemReplication);
  }

  // return *selection;
}