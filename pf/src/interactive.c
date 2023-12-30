#include <errno.h>
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

/* ---------------------------------------------
 _getstring (char *buf, int n)
   get Userinput from stdin
     *buf = pointer to char*
     n    = size of characters including \0
   ---------------------------------------------*/
int _getstring(char *buf, int n) {
  char *tmp;
  tmp = (char *)malloc(sizeof(char) * n);

  printf(">> ");
  if (fgets(tmp, n, stdin) != NULL) {
    debug_print("... in function %s\n", tmp);
    debug_print("... without null character: %ld\n", strlen(tmp));
    strncpy(buf, tmp, n);
    if (!strchr(tmp, '\n')) {
      // consume rest of chars up to '\n'
      int ch;
      while (((ch = getchar()) != EOF) && (ch != '\n')) /* void */
        ;
      if (ch == EOF) /* input error */
        ;
    }
    free(tmp);
  }
  return OK;
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

void get_sid_list(void) {
  char input[SIDLENGTH] = {0};
  int n = 0;
  size_t length;
  system("clear");
  printf("\nInteractive Mode");
  printf("\n================\n\n");
  printf("Enter SID (q to return)?");

  while (_getstring(input, SIDLENGTH) == 0 && input[0] != 'q' &&
         input[1] != '\n' && n < MAX_SID_PER_ENVIRONMENT) {
    printf("Enter SID (q to return)?");
    strncpy(hanasystem[0][0].hanasid[n].sid, input, SIDLENGTH);

    /*TODO
      mcos abfragen mit y/n/Y/N/Yes/No ebenfalls systemReplication
      Erstelle eine Funktion, die das abfragt. Vlt. kann man _getstring
      verwenden

    */

    hanasystem[0][0].hanasid[n].mcos = true;
    hanasystem[0][0].hanasid[n].systemReplication = false;
    length = strlen(input);
    debug_print("... You entered %s with length=%ld\n", input, length);
    n++;
  }

  printf("\nYou entered:\n");
  n--;
  for (int i = 0; i <= n; i++) {
    printf("SID %d, %s  MCOS=%d  SR=%d\n", i, hanasystem[0][0].hanasid[i].sid,
           hanasystem[0][0].hanasid[i].mcos,
           hanasystem[0][0].hanasid[i].systemReplication);
  }

  // return *selection;
}