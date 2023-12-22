#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"

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

int interactive(void) {
  // int selection;

  get_sid_list();

  // selection = get_systemtype_choice();
  // printf("\nYou entered %d\n", selection);

  return 0;
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
    char selection[3];
  size_t length;

  system("clear");

  do {
    printf("\nInteractive Mode");
    printf("\n================\n\n");
    printf("\nEnter SID (blank line to return)?");
    printf("\n>> ");
    scanf("%s", selection);
    length = strlen(selection);
    printf("\nYou entered %s with length=%ld", selection, length);

  } while (length != 0);

  // return *selection;
}