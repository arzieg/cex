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

  // get_sid_list();
  get_system_data();

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
  int number_of_hosts = 6;

  system("clear");
  printf("\nInteractive Mode");
  printf("\n================\n\n");

  do {
    // only temporary
    printf("\nNumber of HANA Hosts in DC1: ");
    scanf(" %d", &number_of_hosts);
    clear_stdin();
    printf("\nPlease enter SID: ");
    CustomString *line =
        custom_getline(stdin, SIDLENGTH + 1, SIDLENGTH + 1, ISALPHANUMERIC);
    strncpy(hanasid[0][n].sid, line->string, line->length);
    hanasid[0][n].sid[line->length - 1] =
        '\0';  // add \0 Terminator at end of SID
    to_upper_case(hanasid[0][n].sid, strnlen(hanasid[0][n].sid, SIDLENGTH));
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
    clear_stdin();

    // todo: change here code to use number of hosts
    printf("\n------------ DC 1 ------------");
    for (int i = 0; i < number_of_hosts; i++) {
      printf("\nRule for each HANA System in DC1: ");
      CustomString *line = custom_getline(stdin, 10, 21, ISALPHA_AND_COLON);
      strncpy(hanasid[0][n].nodes_dc1[i], line->string, line->length);
      hanasid[0][n].nodes_dc1[i][line->length - 1] =
          '\0';  // add \0 Terminator at end of SID
      debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                  line->buffer_size);
      debug_print("Line read:%s\n", line->string);
      free(line);
    };

    printf("\n------------ DC 2 ------------");
    for (int i = 0; i < number_of_hosts; i++) {
      printf("\nRule for each HANA System in DC2: ");
      CustomString *line = custom_getline(stdin, 10, 21, ISALPHA_AND_COLON);
      strncpy(hanasid[0][n].nodes_dc2[i], line->string, line->length);
      hanasid[0][n].nodes_dc2[i][line->length - 1] =
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
    debug_print(
        "InstNr: %d,  UID-SIDADM: %d, UID-SAPADM: %d, GID-SIDSHM: %d, "
        "GID-SAPSYS: %d\n",
        hanasid[0][i].installation_number, hanasid[0][i].uid_sidadm,
        hanasid[0][i].uid_sapadm, hanasid[0][i].gid_sidshm,
        hanasid[0][i].gid_sapsys);

    for (int j = 0; j < number_of_hosts; j++) {
      debug_print("Rule: %s \n", hanasid[0][i].nodes_dc1[j]);
    }
    for (int j = 0; j < number_of_hosts; j++) {
      debug_print("Rule: %s \n", hanasid[0][i].nodes_dc2[j]);
    }
  }
}

void get_system_data(void) {
  int n = 0;

  system("clear");
  printf("\nInteractive Mode");
  printf("\n================\n\n");

  do {
    printf("\nEnter physical Hostname for System %d: ", n);
    CustomString *line =
        custom_getline(stdin, 8, HOSTNAME_LENGTH + 1, ISALPHA_OR_HYPHEN);
    strncpy(hanasystem[0][n].physical_hostname, line->string, line->length);
    hanasystem[0][n].physical_hostname[line->length - 1] =
        '\0';  // add \0 Terminator at end
    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line convert:%s\n", hanasystem[0][n].physical_hostname);
    free(line);

    printf("\nEnter virtual Hostname for System %s: ",
           hanasystem[0][n].physical_hostname);
    line = custom_getline(stdin, 8, HOSTNAME_LENGTH + 1, ISALPHA_OR_HYPHEN);
    strncpy(hanasystem[0][n].virtual_hostname, line->string, line->length);
    hanasystem[0][n].virtual_hostname[line->length - 1] =
        '\0';  // add \0 Terminator at end
    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line convert:%s\n", hanasystem[0][n].virtual_hostname);
    free(line);

    printf("\nEnter 1. MAC Adress for System %s: ",
           hanasystem[0][n].physical_hostname);
    line =
        custom_getline(stdin, MACLENGTH + 1, MACLENGTH + 1, ISAPLHA_OR_COLON);
    strncpy(hanasystem[0][n].mac_address1, line->string, line->length);
    hanasystem[0][n].mac_address1[line->length - 1] =
        '\0';  // add \0 Terminator at end of SID
    to_upper_case(hanasystem[0][n].mac_address1,
                  strnlen(hanasystem[0][n].mac_address1, MACLENGTH));
    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line convert:%s\n", hanasystem[0][n].mac_address1);
    free(line);

    printf("\nEnter 2. MAC Adress for System %s: ",
           hanasystem[0][n].physical_hostname);
    line =
        custom_getline(stdin, MACLENGTH + 1, MACLENGTH + 1, ISAPLHA_OR_COLON);

    strncpy(hanasystem[0][n].mac_address2, line->string, line->length);
    hanasystem[0][n].mac_address2[line->length - 1] =
        '\0';  // add \0 Terminator at end of SID
    to_upper_case(hanasystem[0][n].mac_address2,
                  strnlen(hanasystem[0][n].mac_address2, MACLENGTH));
    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line convert:%s\n", hanasystem[0][n].mac_address2);
    free(line);

    /*
      export PF4SH_SO_01_NET_MACS1_DC1=(AB:68:62:2B:68:4A AB:68:62:2B:67:66
    AB:68:62:2B:67:68) export PF4SH_SO_01_NET_MACS1_DC2=(AB:68:62:2B:68:6C
    AB:68:62:2B:67:58 AB:68:62:2B:66:98) export
    PF4SH_SO_01_NET_MACS2_DC1=(98:45:AB:6A:FB:74 98:45:AB:6A:FB:78
    98:45:AB:6A:FB:58) export PF4SH_SO_01_NET_MACS2_DC2=(98:45:AB:28:31:FC
    98:45:AB:28:32:08 98:45:AB:28:31:D8) export
    PF4SH_SO_01_NET_HOSTS_DC1=(hdb10s04-0001 hdb10s04-0002 hdb10s04-0003) export
    PF4SH_SO_01_NET_HOSTS_DC2=(hdb10s04-1001 hdb10s04-1002 hdb10s04-1003) export
    PF4SH_SO_01_NET_HANA_01_HOSTS_DC1=(lavdb10s04001 lavdb10s04002
    lavdb10s04003) export PF4SH_SO_01_NET_HANA_01_HOSTS_DC2=(lavdb10s04101
    lavdb10s04102 lavdb10s04103) export
    PF4SH_SO_01_NET_HANA_01_IPS_DC1=(10.2.65.0 10.2.65.1 24 665
    1500 10.2.65.201 10.2.65.202 10.2.65.203) export
    PF4SH_SO_01_NET_HANA_01_IPS_DC2=(10.2.65.0 10.2.65.1 24 665
    1500 10.2.65.204 10.2.65.205 10.2.65.206) export
    PF4SH_SO_01_NET_ADM_DC1=(10.2.15.0 10.2.15.1 24 615
    1500 10.2.15.101 10.2.15.102 10.2.15.103) export
    PF4SH_SO_01_NET_ADM_DC2=(10.2.15.0 10.2.15.1 24 615
    1500 10.2.15.104 10.2.15.105 10.2.15.106) export
    PF4SH_SO_01_NET_CLIENT_DC1=(10.2.65.0 10.2.65.1 24 665
    1500 10.2.65.101 10.2.65.102 10.2.65.103) export
    PF4SH_SO_01_NET_CLIENT_DC2=(10.2.65.0 10.2.65.1 24 665
    1500 10.2.65.104 10.2.65.105 10.2.65.106) export
    PF4SH_SO_01_NET_ST_DC1=(10.2.14.0 10.2.14.1 24 614
    9000 10.2.14.141 10.2.14.142 10.2.14.143) export
    PF4SH_SO_01_NET_ST_DC2=(10.2.14.0 10.2.14.1 24 614
    9000 10.2.14.144 10.2.14.145 10.2.14.146) export
    PF4SH_SO_01_NET_CR1_DC1=(10.2.60.0 10.2.60.1 24 660
    1500 10.2.60.101 10.2.60.102 10.2.60.103) export
    PF4SH_SO_01_NET_CR1_DC2=(10.2.60.0 10.2.60.1 24 660
    1500 10.2.60.104 10.2.60.105 10.2.60.106) export
    PF4SH_SO_01_NET_CR2_DC1=(10.2.61.0 10.2.61.1 24 661
    1500 10.2.61.101 10.2.61.102 10.2.61.103) export
    PF4SH_SO_01_NET_CR2_DC2=(10.2.61.0 10.2.61.1 24 661
    1500 10.2.61.104 10.2.61.105 10.2.61.106) export
    PF4SH_SO_01_NET_PCM_DC1=(10.2.63.0 10.2.63.1 24 663
    1500 10.2.63.101 10.2.63.102 10.2.63.103) export
    PF4SH_SO_01_NET_PCM_DC2=(10.2.63.0 10.2.63.1 24 663
    1500 10.2.63.104 10.2.63.105 10.2.63.106) export
    PF4SH_SO_01_NET_BAK_DC1=(10.2.4.0 10.2.4.1 24 400
    1500 10.2.4.101 10.2.4.102 10.2.4.103) export
    PF4SH_SO_01_NET_BAK_DC2=(10.2.4.0 10.2.4.1 24 400
    1500 10.2.4.104 10.2.4.105 10.2.4.106) # first HANA Installation on these
    hosts export PF4SH_SO_01_NET_HANA_01_SE_DC1=(10.2.64.0 10.2.64.1 24 664
    1500 10.2.64.101 10.2.64.102 10.2.64.103) export
    PF4SH_SO_01_NET_HANA_01_SE_DC2=(10.2.64.0 10.2.64.1 24 664
    1500 10.2.64.104 10.2.64.105 10.2.64.106) export
    PF4SH_SO_01_NET_HANA_01_CLIENT_DC1=(10.2.65.0 10.2.65.1 24 665
    1500 10.2.65.101 10.2.65.102 10.2.65.103) export
    PF4SH_SO_01_NET_HANA_01_CLIENT_DC2=(10.2.65.0 10.2.65.1 24 665
    1500 10.2.65.104 10.2.65.105 10.2.65.106) # ScaleOut Intercommunication #
    TAKE CARE!!! MTU should be 9000 für HNR!!!!!!!! export
    PF4SH_SO_01_NET_HANA_01_HNR_DC1=(10.2.62.0 10.2.62.1 24 662
    1500 10.2.62.101 10.2.62.102 10.2.62.103) export
    PF4SH_SO_01_NET_HANA_01_HNR_DC2=(10.2.62.0 10.2.62.1 24 662
    1500 10.2.62.104 10.2.62.105 10.2.62.106) export
    PF4SH_SO_01_NET_HANA_01_HNI_DC1=(10.2.66.0 10.2.66.1 24 666
    9000 10.2.66.101 10.2.66.102 10.2.66.103) export
    PF4SH_SO_01_NET_HANA_01_HNI_DC2=(10.2.66.0 10.2.66.1 24 666
    9000 10.2.66.104 10.2.66.105 10.2.66.106)
    */
  } while (true);
}