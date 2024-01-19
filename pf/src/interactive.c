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

int get_number_systems(void) {
  u_int count_hanasystems;
  bool rc = false;

  do {
    printf("\nHow many HANA Systems are to definied (max = %d) ",
           MAX_HOST_EACH_HANASYSTEM);

    scanf(" %d", &count_hanasystems);
    if (count_hanasystems >= 4 &&
        count_hanasystems <= MAX_HOST_EACH_HANASYSTEM &&
        count_hanasystems % 2 == 0) {
      rc = true;
      break;
    } else {
      printf(
          "\nError: Count of HANA Systems must be even and between 4 (master, "
          "worker) and %d ",
          MAX_HOST_EACH_HANASYSTEM);
    }
  } while (!rc);
  return count_hanasystems;
}

int get_number_sids(void) {
  u_int count_hanasids;
  bool rc = false;

  do {
    printf("\nHow many HANA SIDS are on the System (max = %d) ",
           MAX_SID_PER_ENVIRONMENT);

    scanf(" %d", &count_hanasids);
    if (count_hanasids >= 1 && count_hanasids <= MAX_SID_PER_ENVIRONMENT) {
      rc = true;
      break;
    } else {
      printf("\nError: Count of HANA SIDs must be between 1 and %d ",
             MAX_SID_PER_ENVIRONMENT);
    }
  } while (!rc);
  return count_hanasids;
}

int interactive(void) {
  // int selection;

  // get_sid_list();
  // get_system_data();

  get_systemtype_choice();
  // printf("\nYou entered %d\n", selection);

  return OK;
}

void get_systemtype_choice(void) {
  int selection = 0;
  int count_hanasystems = 0;
  int count_hanasids = 0;

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
    case 1:
      count_hanasystems = get_number_systems();
      count_hanasids = get_number_sids();
      clear_stdin();
      get_system_data(count_hanasystems);
      break;

    case 5:
      printf("\nGood Bye\n");
      exit(0);
      break;

    default:
      break;
  }
}

/*
  get_network_name(network)
    get information from network environment
*/
NETWORKTYPE get_network_name(char *network_name, char *hostname) {
  NETWORKTYPE nw;

  printf("\nInformation for Network %s for Host %s ", network_name, hostname);
  printf("\n-------------------------------------------------");
  printf("\nNetwork IP:");

  strcpy(nw.network_name, network_name);

  CustomString *line = custom_getline(stdin, IP_MIN_LENGTH + 1,
                                      IP_MAX_LENGTH + 1, ISALPHA_OR_DOT);
  strncpy(nw.network_ip, line->string, line->length);
  nw.network_ip[line->length - 1] = '\0';
  free(line);

  printf("\nGateway IP:");
  line = custom_getline(stdin, IP_MIN_LENGTH + 1, IP_MAX_LENGTH + 1,
                        ISALPHA_OR_DOT);
  strncpy(nw.network_gw, line->string, line->length);
  nw.network_gw[line->length - 1] = '\0';
  free(line);

  printf("\nIP of Host in this subnet:");
  line = custom_getline(stdin, IP_MIN_LENGTH + 1, IP_MAX_LENGTH + 1,
                        ISALPHA_OR_DOT);
  strncpy(nw.network_host_ip, line->string, line->length);
  nw.network_host_ip[line->length - 1] = '\0';
  free(line);

  printf("\nNetwork Mask: ");
  scanf(" %hhd", &nw.network_netmask);

  printf("\nNetwork VLAN-ID: ");
  scanf(" %hd", &nw.network_vlanid);

  printf("\nNetwork MTU Size: ");
  scanf(" %hd", &nw.network_mtu);

  clear_stdin();

  return nw;
}

/* --------------------------------------------------------------
 Enter the SID list. Each SID could be part of a systemReplication.
 If more then one System is entered, the system is per definition part of a
 mcos system
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

void get_system_data(int max_hana_systems) {
  int n = 0;
  NETWORKTYPE nwtmp;
  char dc[4] = "DC1";

  system("clear");
  printf("\nInteractive Mode");
  printf("\n================\n\n");

  do {
    if (n >= max_hana_systems / 2) {
      strcpy(dc, "DC2");
    }

    printf("\nEnter physical Hostname in %s, SystemID %d: ", dc, n);
    CustomString *line =
        custom_getline(stdin, 8, HOSTNAME_LENGTH + 1, ISALPHA_OR_HYPHEN);
    strncpy(hanasystem[0][n].physical_hostname, line->string, line->length);
    hanasystem[0][n].physical_hostname[line->length - 1] =
        '\0';  // add \0 Terminator at end
    debug_print("Read %zd bytes, buffer is %zd bytes\n", line->length,
                line->buffer_size);
    debug_print("Line convert:%s\n", hanasystem[0][n].physical_hostname);
    free(line);

    printf("\nEnter virtual Hostname in %s, SystemID %s: ", dc,
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
    n++;
  } while (n < max_hana_systems);

  nwtmp = get_network_name("IPS", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_ips.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_ips.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_ips.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_ips.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_ips.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_ips.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_ips.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("ADM", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_adm.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_adm.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_adm.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_adm.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_adm.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_adm.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_adm.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("CLIENT", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_client.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_client.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_client.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_client.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_client.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_client.network_host_ip,
         nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_client.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("ST", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_st.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_st.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_st.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_st.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_st.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_st.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_st.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("CR1", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_cr1.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_cr1.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_cr1.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_cr1.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_cr1.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_cr1.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_cr1.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("CR2", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_cr2.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_cr2.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_cr2.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_cr2.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_cr2.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_cr2.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_cr2.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("PCM", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_pcm.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_pcm.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_pcm.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_pcm.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_pcm.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_pcm.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_pcm.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("BAK", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_bak.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_bak.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_bak.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_bak.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_bak.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_bak.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_bak.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("SE", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_se.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_se.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_se.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_se.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_se.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_se.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_se.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("HNR", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_hnr.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_hnr.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_hnr.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_hnr.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_hnr.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_hnr.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_hnr.network_ip, nwtmp.network_ip);

  nwtmp = get_network_name("HNI", hanasystem[0][n].physical_hostname);
  hanasystem[0][n].network_hni.network_mtu = nwtmp.network_mtu;
  hanasystem[0][n].network_hni.network_netmask = nwtmp.network_netmask;
  hanasystem[0][n].network_hni.network_vlanid = nwtmp.network_vlanid;
  strcpy(hanasystem[0][n].network_hni.network_name, nwtmp.network_name);
  strcpy(hanasystem[0][n].network_hni.network_gw, nwtmp.network_gw);
  strcpy(hanasystem[0][n].network_hni.network_host_ip, nwtmp.network_host_ip);
  strcpy(hanasystem[0][n].network_hni.network_ip, nwtmp.network_ip);
}