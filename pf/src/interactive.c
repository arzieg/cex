#define _GNU_SOURCE  // to use getline, define before stdio.h
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"
#include "pfhelper.h"

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
    CustomString *line = custom_getline(stdin, SIDLENGTH + 1, SIDLENGTH + 1);
    strncpy(hanasid[0][n].sid, line->string, line->length);
    hanasid[0][n].sid[line->length - 1] =
        '\0';  // add \0 Terminator at end of SID
    toUpperCase(hanasid[0][n].sid, strnlen(hanasid[0][n].sid, SIDLENGTH));
    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line read:%s\n", line->string);
    free(line);

    strncpy(msg, "is the SAP SID part of a System Replication (Y/N)?", 255);
    systemreplication = get_yesno_status(msg, stdin);
    debug_print("... SR = %d\n", systemreplication);
    hanasid[0][n].systemReplication = systemreplication;

    printf("\nInstallationnumber: ");
    scanf("%d", &hanasid[0][n].installation_number);
    printf("\nUID of SIDADM: ");
    scanf("%d", &hanasid[0][n].uid_sidadm);
    printf("\nUID of SAPADM: ");
    scanf("%d", &hanasid[0][n].uid_sapadm);
    printf("\nGID of SIDSHM: ");
    scanf("%d", &hanasid[0][n].gid_sidshm);
    printf("\nGID of SAPSYS: ");
    scanf("%d", &hanasid[0][n].gid_sapsys);

    // todo: change here code to use number of hosts / 2
    for (int i = 0; i < 3;
         i++) {  // iterate over each host each side - fix value for test = 3
      printf("Rule for each HANA System: ");
      CustomString *line = custom_getline(stdin, 10, 21);
      strncpy(hanasid[0][n].nodes_dc1[i], line->string, line->length);
      hanasid[0][n].nodes_dc1[i][line->length - 1] =
          '\0';  // add \0 Terminator at end of SID
      debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                  line->buffer_size);
      debug_print("Line read:%s\n", line->string);
      free(line);
    };

    printf("\nNumber of storage partitions (data+log volumes): ");
    scanf("%d", &hanasid[0][n].num_numsp);

    n++;

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

  n--;
  bool mcos;
  (n > 0) ? (mcos = true) : (mcos = false);

  for (int i = 0; i <= n; i++) {
    debug_print("SID %d, %s  MCOS=%d  SR=%d\n", i, hanasid[0][i].sid, mcos,
                hanasid[0][i].systemReplication);
  }

  // return *selection;
}