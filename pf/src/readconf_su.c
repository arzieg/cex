// implicit declaration of function getline
#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"
#include "pf.h"
#include "pfhelper.h"

#define REGEXMAC "\\((([a-fA-F0-9]{2}[:-]){5}([a-fA-F0-9]{2}))\\)"
#define REGEXBRACKETSTR "\\(([^()]*)\\)"
#define REGEXSTR "\"([^\"]*)\""
#define REGEXMATCHWORD "(\\w+)"
#define REGEXMATCHIP "(\\w+.\\w+.\\w+.\\w+)"
#define REGEXNUM "(\\b[1-9]\\b)"        // find Numbers
#define REGEXMATCHVAR "(\\$\\w+|\\w+)"  // find $VAR
#define FREE(ptr) \
  do {            \
    free(ptr);    \
    ptr = NULL;   \
  } while (0)

extern HANASYSTEMTYPE hanasystem[MAX_ENVIRONMENTS][MAX_HOST_EACH_HANASYSTEM];
extern SIDTYPE hanasid[MAX_ENVIRONMENTS][MAX_HOST_EACH_HANASYSTEM];
extern PMKHANATYPE pmkhana[MAX_ENVIRONMENTS];

NETWORKTYPE *phanasystem;

struct ConfigTableArray ConfigTable[] = {
    {.parametername = "SU_NET_MACS1_DC2",
     .regexeccode = "SU_NET_MACS1_DC2=" REGEXBRACKETSTR "",
     .maxlength = MACLENGTH,
     .index = 1},
    {.parametername = "SU_NET_MACS1_DC1",
     .regexeccode = "SU_NET_MACS1_DC1=" REGEXBRACKETSTR "",
     .maxlength = MACLENGTH,
     .index = 2},
    {.parametername = "SU_NET_MACS2_DC2",
     .regexeccode = "SU_NET_MACS2_DC2=" REGEXBRACKETSTR "",
     .maxlength = MACLENGTH,
     .index = 3},
    {.parametername = "SU_NET_MACS2_DC1",
     .regexeccode = "SU_NET_MACS2_DC1=" REGEXBRACKETSTR "",
     .maxlength = MACLENGTH,
     .index = 4},
    {.parametername = "SU_NET_HOSTS_DC1",
     .regexeccode = "SU_NET_HOSTS_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 5},
    {.parametername = "SU_NET_HOSTS_DC2",
     .regexeccode = "SU_NET_HOSTS_DC2=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 6},
    {.parametername = "SU_NET_HANA_01_HOSTS_DC1",
     .regexeccode = "SU_NET_HANA_01_HOSTS_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 7},
    {.parametername = "SU_NET_HANA_01_HOSTS_DC2",
     .regexeccode = "SU_NET_HANA_01_HOSTS_DC2=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 8},
    {.parametername = "SU_NET_HANA_02_HOSTS_DC1",
     .regexeccode = "SU_NET_HANA_02_HOSTS_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 9},
    {.parametername = "SU_NET_HANA_02_HOSTS_DC2",
     .regexeccode = "SU_NET_HANA_02_HOSTS_DC2=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 10},
    {.parametername = "SU_NET_HANA_01_IPS_DC1",
     .regexeccode = "SU_NET_HANA_01_IPS_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 11},
    {.parametername = "SU_NET_HANA_01_IPS_DC2",
     .regexeccode = "SU_NET_HANA_01_IPS_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 12},
    {.parametername = "SU_NET_HANA_02_IPS_DC1",
     .regexeccode = "SU_NET_HANA_02_IPS_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 13},
    {.parametername = "SU_NET_HANA_02_IPS_DC2",
     .regexeccode = "SU_NET_HANA_02_IPS_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 14},
    {.parametername = "SU_NET_ADM_DC1",
     .regexeccode = "SU_NET_ADM_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 15},
    {.parametername = "SU_NET_ADM_DC2",
     .regexeccode = "SU_NET_ADM_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 16},
    {.parametername = "SU_NET_CLIENT_DC1",
     .regexeccode = "SU_NET_CLIENT_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 17},
    {.parametername = "SU_NET_CLIENT_DC2",
     .regexeccode = "SU_NET_CLIENT_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 18},
    {.parametername = "SU_NET_ST_DC1",
     .regexeccode = "SU_NET_ST_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 19},
    {.parametername = "SU_NET_ST_DC2",
     .regexeccode = "SU_NET_ST_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 20},
    {.parametername = "SU_NET_CR1_DC1",
     .regexeccode = "SU_NET_CR1_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 21},
    {.parametername = "SU_NET_CR1_DC2",
     .regexeccode = "SU_NET_CR1_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 22},
    {.parametername = "SU_NET_CR2_DC1",
     .regexeccode = "SU_NET_CR2_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 23},
    {.parametername = "SU_NET_CR2_DC2",
     .regexeccode = "SU_NET_CR2_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 24},
    {.parametername = "SU_NET_PCM_DC1",
     .regexeccode = "SU_NET_PCM_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 25},
    {.parametername = "SU_NET_PCM_DC2",
     .regexeccode = "SU_NET_PCM_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 26},
    {.parametername = "SU_NET_BAK_DC1",
     .regexeccode = "SU_NET_BAK_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 27},
    {.parametername = "SU_NET_BAK_DC2",
     .regexeccode = "SU_NET_BAK_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 28},
    {.parametername = "SU_NET_HANA_01_SE_DC1",
     .regexeccode = "SU_NET_HANA_01_SE_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 29},
    {.parametername = "SU_NET_HANA_01_SE_DC2",
     .regexeccode = "SU_NET_HANA_01_SE_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 30},
    {.parametername = "SU_NET_HANA_01_CLIENT_DC1",
     .regexeccode = "SU_NET_HANA_01_CLIENT_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 31},
    {.parametername = "SU_NET_HANA_01_CLIENT_DC2",
     .regexeccode = "SU_NET_HANA_01_CLIENT_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 32},
    {.parametername = "SU_NET_HANA_02_SE_DC1",
     .regexeccode = "SU_NET_HANA_02_SE_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 33},
    {.parametername = "SU_NET_HANA_02_SE_DC2",
     .regexeccode = "SU_NET_HANA_02_SE_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 34},
    {.parametername = "SU_NET_HANA_02_CLIENT_DC1",
     .regexeccode = "SU_NET_HANA_02_CLIENT_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 35},
    {.parametername = "SU_NET_HANA_02_CLIENT_DC2",
     .regexeccode = "SU_NET_HANA_02_CLIENT_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 36},
    {.parametername = "SU_NET_HANA_01_HNR_DC1",
     .regexeccode = "SU_NET_HANA_01_HNR_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 37},
    {.parametername = "SU_NET_HANA_01_HNR_DC2",
     .regexeccode = "SU_NET_HANA_01_HNR_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 38},
    {.parametername = "SU_NET_HANA_02_HNR_DC1",
     .regexeccode = "SU_NET_HANA_02_HNR_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 39},
    {.parametername = "SU_NET_HANA_02_HNR_DC2",
     .regexeccode = "SU_NET_HANA_02_HNR_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 40},
    {.parametername = "SU_VIRTUAL",
     .regexeccode = "SU_VIRTUAL=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 41},
    {.parametername = "SU_HANA_SID",
     .regexeccode = "SU_HANA_SID=" REGEXBRACKETSTR "",
     .maxlength = SIDLENGTH * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 42},
    {.parametername = "SU_HANA_INR",
     .regexeccode = "SU_HANA_INR=" REGEXBRACKETSTR "",
     .maxlength = SIDLENGTH * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 43},
    {.parametername = "SU_HANA_ALLOC_LIMIT_GB",
     .regexeccode = "SU_HANA_ALLOC_LIMIT_GB=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 44},
    {.parametername = "SU_HANA_PRELOAD_TABLES",
     .regexeccode = "SU_HANA_PRELOAD_TABLES=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 45},
    {.parametername = "SU_UID_SIDADM",
     .regexeccode = "SU_UID_SIDADM=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 46},
    {.parametername = "SU_UID_SAPADM",
     .regexeccode = "SU_UID_SAPADM=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 47},
    {.parametername = "SU_GID_SIDSHM",
     .regexeccode = "SU_GID_SIDSHM=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 48},
    {.parametername = "SU_GID_SAPSYS",
     .regexeccode = "SU_GID_SAPSYS=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 49},
    {.parametername = "SU_UID_SAPROUTER",
     .regexeccode = "SU_UID_SAPROUTER=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 50},
    {.parametername = "SU_HANA_01_NODES_DC1",
     .regexeccode = "SU_HANA_01_NODES_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 51},
    {.parametername = "SU_HANA_01_NODES_DC2",
     .regexeccode = "SU_HANA_01_NODES_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 52},
    {.parametername = "SU_HANA_02_NODES_DC1",
     .regexeccode = "SU_HANA_02_NODES_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 53},
    {.parametername = "SU_HANA_02_NODES_DC2",
     .regexeccode = "SU_HANA_02_NODES_DC2=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 54},
    {.parametername = "SU_HANA_01_DC1_NAME",
     .regexeccode = "SU_HANA_01_DC1_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 55},
    {.parametername = "SU_HANA_01_DC2_NAME",
     .regexeccode = "SU_HANA_01_DC2_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 56},
    {.parametername = "SU_HANA_02_DC1_NAME",
     .regexeccode = "SU_HANA_02_DC1_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 57},
    {.parametername = "SU_HANA_02_DC2_NAME",
     .regexeccode = "SU_HANA_02_DC2_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 58},
    {.parametername = "SU_XHANA2_RELEASE",
     .regexeccode = "SU_XHANA2_RELEASE=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 59},
    {.parametername = "SU_HANA_01_SAPREPO_VERSION_DC1",
     .regexeccode = "SU_HANA_01_SAPREPO_VERSION_DC1=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 60},
    {.parametername = "SU_HANA_01_SAPREPO_VERSION_DC2",
     .regexeccode = "SU_HANA_01_SAPREPO_VERSION_DC2=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 61},
    {.parametername = "SU_HANA_02_SAPREPO_VERSION_DC1",
     .regexeccode = "SU_HANA_02_SAPREPO_VERSION_DC1=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 62},
    {.parametername = "SU_HANA_02_SAPREPO_VERSION_DC2",
     .regexeccode = "SU_HANA_02_SAPREPO_VERSION_DC2=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 63},
    {.parametername = "SU_HANA_01_NAS_SVMS",
     .regexeccode = "SU_HANA_01_NAS_SVMS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 64},
    {.parametername = "SU_HANA_02_NAS_SVMS",
     .regexeccode = "SU_HANA_02_NAS_SVMS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 65},
    {.parametername = "SU_HANA_01_NAS_PORTS",
     .regexeccode = "SU_HANA_01_NAS_PORTS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 66},
    {.parametername = "SU_HANA_02_NAS_PORTS",
     .regexeccode = "SU_HANA_02_NAS_PORTS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 67},
    {.parametername = "SU_HANA_01_NAS_NUMSP",
     .regexeccode = "SU_HANA_01_NAS_NUMSP=" REGEXNUM "",
     .maxlength = TEXTLENGTH,
     .index = 68},
    {.parametername = "SU_HANA_02_NAS_NUMSP",
     .regexeccode = "SU_HANA_02_NAS_NUMSP=" REGEXNUM "",
     .maxlength = TEXTLENGTH,
     .index = 69},
    {.parametername = "SU_HANA_01_NAS_COMMENT",
     .regexeccode = "SU_HANA_01_NAS_COMMENT=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 70},
    {.parametername = "SU_HANA_02_NAS_COMMENT",
     .regexeccode = "SU_HANA_02_NAS_COMMENT=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 71},
    {.parametername = "SU_HANA_01_VIRTUAL_HOST_01",
     .regexeccode = "SU_HANA_01_VIRTUAL_HOST_01=" REGEXMATCHWORD "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 72},
    {.parametername = "SU_HANA_01_VIRTUAL_IP_01",
     .regexeccode = "SU_HANA_01_VIRTUAL_IP_01=" REGEXMATCHIP "",
     .maxlength = IP_MAX_LENGTH,
     .index = 73},
    {.parametername = "SU_HANA_01_VIRTUAL_GW_01",
     .regexeccode = "SU_HANA_01_VIRTUAL_GW_01=" REGEXMATCHIP "",
     .maxlength = IP_MAX_LENGTH,
     .index = 74},
    {.parametername = "SU_HANA_01_VIRTUAL_NETMASK_01",
     .regexeccode = "SU_HANA_01_VIRTUAL_NETMASK_01=" REGEXMATCHWORD "",
     .maxlength = TEXTLENGTH,
     .index = 75},
    {.parametername = "SU_HANA_01_VIRTUAL_VLAN_01",
     .regexeccode = "SU_HANA_01_VIRTUAL_VLAN_01=" REGEXMATCHWORD "",
     .maxlength = TEXTLENGTH,
     .index = 76},
    {.parametername = "SU_HANA_01_VIRTUAL_HOST_02",
     .regexeccode = "SU_HANA_01_VIRTUAL_HOST_02=" REGEXMATCHWORD "",
     .maxlength = HOSTNAME_LENGTH,
     .index = 77},
    {.parametername = "SU_HANA_01_VIRTUAL_IP_02",
     .regexeccode = "SU_HANA_01_VIRTUAL_IP_02=" REGEXMATCHIP "",
     .maxlength = IP_MAX_LENGTH,
     .index = 78},
    {.parametername = "SU_HANA_01_VIRTUAL_GW_02",
     .regexeccode = "SU_HANA_01_VIRTUAL_GW_02=" REGEXMATCHIP "",
     .maxlength = IP_MAX_LENGTH,
     .index = 79},
    {.parametername = "SU_HANA_01_VIRTUAL_NETMASK_02",
     .regexeccode = "SU_HANA_01_VIRTUAL_NETMASK_02=" REGEXMATCHWORD "",
     .maxlength = TEXTLENGTH,
     .index = 80},
    {.parametername = "SU_HANA_01_VIRTUAL_VLAN_02",
     .regexeccode = "SU_HANA_01_VIRTUAL_VLAN_02=" REGEXMATCHWORD "",
     .maxlength = TEXTLENGTH,
     .index = 81},
    {.parametername = "SU_HANA_01_PACE_NODES",
     .regexeccode = "SU_HANA_01_PACE_NODES=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 82},
    {.parametername = "SU_HANA_01_PACE_ISCSI_TARGETS",
     .regexeccode = "SU_HANA_01_PACE_ISCSI_TARGETS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 83},
    {.parametername = "SU_HANA_IRMC_USER",
     .regexeccode = "SU_HANA_IRMC_USER=" REGEXMATCHWORD "",
     .maxlength = TEXTLENGTH,
     .index = 84},
    {.parametername = "SU_HANA_IRMC_PWD",
     .regexeccode = "SU_HANA_IRMC_PWD=" REGEXMATCHVAR "",
     .maxlength = TEXTLENGTH,
     .index = 85},
};

/*
  ----------------------------------------
  search pattern in text and return result
  -----------------------------------------
*/
char *find_matches(const char *pattern, const char *text) {
  // https://gist.github.com/ianmackinnon/3294587

  size_t maxGroups = 3;
  size_t reti;

  regex_t regexCompiled;
  regmatch_t groupArray[maxGroups];
  char *cursor = NULL;

  if (regcomp(&regexCompiled, pattern, REG_EXTENDED)) {
    printf("Could not compile regular expression.\n");
    return NULL;
  };

  reti = regexec(&regexCompiled, text, maxGroups, groupArray, 0);

  if (!reti) {
    regoff_t off, len;
    off = groupArray[1].rm_so;
    len = groupArray[1].rm_eo - groupArray[1].rm_so;
    cursor = (char *)calloc((len + 1), sizeof(char));
    // cursor = (char *)malloc((len+1) * sizeof(char));   # hier wird bei
    // kleinen Speicher ein Byte mehr allokiert als angefordert
    strncpy(cursor, text + off, len);
    cursor[len + 1] = '\0';
    // debug_print("\n found = %s\n", cursor);
  }

  regfree(&regexCompiled);

  return (cursor != NULL) ? cursor : NULL;
}

/*
  split line and assign to networktype
*/
void split_line_and_assign_to_networktype(char *line,
                                          NETWORKTYPE *phanasystem) {
  char **token_array = split_string(line, ' ');

  int cnt = 0;

  if (token_array) {
    for (int i = 0; token_array[i] != NULL; i++) {
      // debug_print("\n Address = %p", token_array[i]);
      cnt++;
    }
  }
  // debug_print("\nCount=%d\n", cnt);

  if (token_array) {
    for (int i = 0; token_array[i] != NULL; i++) {
      // debug_print("Token %d: [%s]\n", i, token_array[i]);
      switch (i) {
        case 0:
          strncpy(phanasystem->network_ip, token_array[i], 15);
          break;
        case 1:
          strncpy(phanasystem->network_gw, token_array[1], 15);
          break;
        case 2:
          phanasystem->network_netmask = (uint8_t)atoi(token_array[i]);
          break;
        case 3:
          phanasystem->network_vlanid = (uint16_t)atoi(token_array[i]);
          break;
        case 4:
          phanasystem->network_mtu = (uint16_t)atoi(token_array[i]);
          break;
        case 5:
          strncpy(phanasystem->network_host_ip, token_array[i], 15);
          break;
      }
      free(token_array[i]);
    }
    free(token_array);
  }
}

int get_values(int configtablecount, int environmentindex, int maxhost) {
  /*
  Durchlaufe eine Schleife 0-n von ConfigTable[i].parametername
  case
    xxx setze wert
      wenn wert noch eine zusammenhängende Zeile, dann aufteilen
  */
  for (size_t i = 0; i < configtablecount; i++) {
    char **line;
    if (ConfigTable[i].result != NULL)
      debug_print("\nConfigTable[%d].result = %s  length=%d\n", i,
                  ConfigTable[i].result, strlen(ConfigTable[i].result));

    if (ConfigTable[i].result != NULL) {
      switch (ConfigTable[i].index) {
        case 1:  // SU_NET_MACS1_DC2
          strncpy(hanasystem[environmentindex][1].mac_address1,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 2:  // SU_NET_MACS1_DC1
          strncpy(hanasystem[environmentindex][0].mac_address1,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 3:  // SU_NET_MACS2_DC2
          strncpy(hanasystem[environmentindex][1].mac_address2,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 4:  // SU_NET_MACS2_DC1
          strncpy(hanasystem[environmentindex][0].mac_address2,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 5:  // SU_NET_HOSTS_DC1
          strncpy(hanasystem[environmentindex][0].physical_hostname,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 6:  // SU_NET_HOSTS_DC2
          strncpy(hanasystem[environmentindex][1].physical_hostname,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 7:  // SU_NET_HANA_01_HOSTS_DC1
          strncpy(hanasystem[environmentindex][0].vhostname[0].virtual_hostname,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 8:  // SU_NET_HANA_01_HOSTS_DC2
          strncpy(hanasystem[environmentindex][1].vhostname[0].virtual_hostname,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 9:  // SU_NET_HANA_02_HOSTS_DC1
          strncpy(hanasystem[environmentindex][0].vhostname[1].virtual_hostname,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 10:  // SU_NET_HANA_02_HOSTS_DC2
          strncpy(hanasystem[environmentindex][1].vhostname[1].virtual_hostname,
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          break;
        case 11:  // SU_NET_HANA_01_IPS_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[0].network_ips;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 12:  // SU_NET_HANA_01_IPS_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[0].network_ips;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 13:  // SU_NET_HANA_02_IPS_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[1].network_ips;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 14:  // SU_NET_HANA_02_IPS_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[1].network_ips;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 15:  // SU_NET_ADM_DC1
          phanasystem = &hanasystem[environmentindex][0].network_adm;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 16:  // SU_NET_ADM_DC2
          phanasystem = &hanasystem[environmentindex][1].network_adm;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 17:  // SU_NET_CLIENT_DC1
          phanasystem = &hanasystem[environmentindex][0].network_client;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 18:  // SU_NET_CLIENT_DC2
          phanasystem = &hanasystem[environmentindex][1].network_client;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 19:  // SU_NET_ST_DC1
          phanasystem = &hanasystem[environmentindex][0].network_st;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 20:  // SU_NET_ST_DC2
          phanasystem = &hanasystem[environmentindex][1].network_st;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 21:  // SU_NET_CR1_DC1
          phanasystem = &hanasystem[environmentindex][0].network_cr1;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 22:  // SU_NET_CR1_DC2
          phanasystem = &hanasystem[environmentindex][1].network_cr1;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 23:  // SU_NET_CR2_DC1
          phanasystem = &hanasystem[environmentindex][0].network_cr2;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 24:  // SU_NET_CR2_DC2
          phanasystem = &hanasystem[environmentindex][1].network_cr2;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 25:  // SU_NET_PCM_DC1
          phanasystem = &hanasystem[environmentindex][0].network_pcm;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 26:  // SU_NET_PCM_DC2
          phanasystem = &hanasystem[environmentindex][1].network_pcm;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 27:  // SU_NET_BAK_DC1
          phanasystem = &hanasystem[environmentindex][0].network_bak;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 28:  // SU_NET_BAK_DC2
          phanasystem = &hanasystem[environmentindex][1].network_bak;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 29:  // SU_NET_HANA_01_SE_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[0].network_se;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 30:  // SU_NET_HANA_01_SE_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[0].network_se;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 31:  // SU_NET_HANA_01_CLIENT_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[0].network_client_sid;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 32:  // SU_NET_HANA_01_CLIENT_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[0].network_client_sid;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 33:  // SU_NET_HANA_02_SE_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[1].network_se;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 34:  // SU_NET_HANA_02_SE_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[1].network_se;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 35:  // SU_NET_HANA_02_CLIENT_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[1].network_client_sid;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 36:  // SU_NET_HANA_02_CLIENT_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[1].network_client_sid;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 37:  // SU_NET_HANA_01_HNR_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[0].network_hnr;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 38:  // SU_NET_HANA_01_HNR_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[0].network_hnr;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 39:  // SU_NET_HANA_02_HNR_DC1
          phanasystem =
              &hanasystem[environmentindex][0].vhostname[1].network_hnr;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
        case 40:  // SU_NET_HANA_02_HNR_DC2
          phanasystem =
              &hanasystem[environmentindex][1].vhostname[1].network_hnr;
          split_line_and_assign_to_networktype(ConfigTable[i].result,
                                               phanasystem);
          break;
          /*
            case 41
          */
        case 42:  // SU_HANA_SID
          line = split_string(ConfigTable[i].result, ' ');

          (line[0] != NULL) ? strncpy(hanasid[environmentindex][0].sid, line[0],
                                      ConfigTable[i].maxlength)
                            : NULL;

          (line[1] != NULL) ? strncpy(hanasid[environmentindex][1].sid, line[1],
                                      ConfigTable[i].maxlength)
                            : NULL;

          debug_print("\nSID %d = %s\n", 0, hanasid[environmentindex][0].sid);
          debug_print("\nSID %d = %s\n", 1, hanasid[environmentindex][1].sid);

          FREE(line);
          break;
        case 43:  // SU_HANA_INR
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0 && tmp < 99) {
              hanasid[environmentindex][0].installation_number = tmp;
            } else {
              hanasid[environmentindex][0].installation_number = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0 && tmp < 99) {
              hanasid[environmentindex][1].installation_number = tmp;
            } else {
              hanasid[environmentindex][1].installation_number = -1;
            }
          }

          debug_print("\nINR 0 = %d\n",
                      hanasid[environmentindex][0].installation_number);
          debug_print("\nINR 1 = %d\n",
                      hanasid[environmentindex][1].installation_number);

          FREE(line);
          break;
        case 44:  // SU_HANA_ALLOC_LIMIT_GB
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0) {
              hanasid[environmentindex][0].allocation_limit = tmp;
            } else {
              hanasid[environmentindex][0].allocation_limit = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0) {
              hanasid[environmentindex][1].allocation_limit = tmp;
            } else {
              hanasid[environmentindex][1].allocation_limit = -1;
            }
          }

          debug_print("\nALLOC LIMIT 0 = %d\n",
                      hanasid[environmentindex][0].allocation_limit);
          debug_print("\nALLOC LIMIT 1 = %d\n",
                      hanasid[environmentindex][1].allocation_limit);

          FREE(line);
          break;
        case 45:  // SU_HANA_PRELOAD_TABLES
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL && strcmp(line[0], "true")) {
            hanasid[environmentindex][0].preload = true;
          } else {
            hanasid[environmentindex][0].preload = false;
          }

          if (line[1] != NULL && strcmp(line[1], "true")) {
            hanasid[environmentindex][1].preload = true;
          } else {
            hanasid[environmentindex][1].preload = false;
          }

          debug_print("\nPRELOAD 0 = %d\n",
                      hanasid[environmentindex][0].preload);
          debug_print("\nPRELOAD 1 = %d\n",
                      hanasid[environmentindex][1].preload);

          FREE(line);
          break;
        case 46:  // SU_UID_SIDADM
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][0].uid_sidadm = tmp;
            } else {
              hanasid[environmentindex][0].uid_sidadm = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][1].uid_sidadm = tmp;
            } else {
              hanasid[environmentindex][1].uid_sidadm = -1;
            }
          }

          debug_print("\nSIDADM 0 = %d\n",
                      hanasid[environmentindex][0].uid_sidadm);
          debug_print("\nSIDADM 1 = %d\n",
                      hanasid[environmentindex][1].uid_sidadm);

          FREE(line);
          break;
        case 47:  // SU_UID_SAPADM
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][0].uid_sapadm = tmp;
            } else {
              hanasid[environmentindex][0].uid_sapadm = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][1].uid_sapadm = tmp;
            } else {
              hanasid[environmentindex][1].uid_sapadm = -1;
            }
          }

          debug_print("\nSAPADM 0 = %d\n",
                      hanasid[environmentindex][0].uid_sapadm);
          debug_print("\nSAPADM 1 = %d\n",
                      hanasid[environmentindex][1].uid_sapadm);

          FREE(line);
          break;
        case 48:  // SU_GID_SIDSHM
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][0].gid_sidshm = tmp;
            } else {
              hanasid[environmentindex][0].gid_sidshm = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][1].gid_sidshm = tmp;
            } else {
              hanasid[environmentindex][1].gid_sidshm = -1;
            }
          }

          debug_print("\nGID_SIDSHM 0 = %d\n",
                      hanasid[environmentindex][0].gid_sidshm);
          debug_print("\nGID_SIDSHM 1 = %d\n",
                      hanasid[environmentindex][1].gid_sidshm);

          FREE(line);
          break;
        case 49:  // SU_GID_SAPSYS
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][0].gid_sidshm = tmp;
            } else {
              hanasid[environmentindex][0].gid_sidshm = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][1].gid_sidshm = tmp;
            } else {
              hanasid[environmentindex][1].gid_sidshm = -1;
            }
          }

          debug_print("\nGID_SAPSYS 0 = %d\n",
                      hanasid[environmentindex][0].gid_sidshm);
          debug_print("\nGID_SAPSYS 1 = %d\n",
                      hanasid[environmentindex][1].gid_sidshm);

          FREE(line);
          break;
        case 50:  // SU_UID_SAPROUTER
          line = split_string(ConfigTable[i].result, ' ');

          if (line[0] != NULL) {
            int tmp = atoi(line[0]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][0].uid_saprouter = tmp;
            } else {
              hanasid[environmentindex][0].uid_saprouter = -1;
            }
          }
          if (line[1] != NULL) {
            int tmp = atoi(line[1]);
            if (tmp > 0 && tmp < 65535) {
              hanasid[environmentindex][1].uid_saprouter = tmp;
            } else {
              hanasid[environmentindex][1].uid_saprouter = -1;
            }
          }

          debug_print("\nSAPROUTER 0 = %d\n",
                      hanasid[environmentindex][0].uid_saprouter);
          debug_print("\nSAPROUTER 1 = %d\n",
                      hanasid[environmentindex][1].uid_saprouter);

          FREE(line);
          break;
        case 51:  // SU_HANA_01_NODES_DC1
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 21) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].nodes_dc1[0], tmp,
                    strlen(tmp));
            debug_print("\nSU_HANA_01_NODES_DC1 = %s\n",
                        hanasid[environmentindex][0].nodes_dc1[0]);
          }
          break;
        case 52:  // SU_HANA_01_NODES_DC2
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 21) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][1].nodes_dc2[0], tmp,
                    strlen(tmp));
            debug_print("\nSU_HANA_01_NODES_DC2 = %s\n",
                        hanasid[environmentindex][1].nodes_dc2[0]);
          }
          break;
        case 53:  // SU_HANA_02_NODES_DC1
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 21) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].nodes_dc1[1], tmp,
                    strlen(tmp));
            debug_print("\nSU_HANA_02_NODES_DC1 = %s\n",
                        hanasid[environmentindex][0].nodes_dc1[1]);
          }
          break;
        case 54:  // SU_HANA_02_NODES_DC2
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 21) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].nodes_dc2[1], tmp,
                    strlen(tmp));
            debug_print("\nSU_HANA_02_NODES_DC1 = %s\n",
                        hanasid[environmentindex][0].nodes_dc2[1]);
          }
          break;
        case 55:  // SU_HANA_01_DC1_NAME
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 5) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].dc1_name[0], tmp, strlen(tmp));
            debug_print("\nSU_HANA_01_DC1_NAME = %s\n",
                        hanasid[environmentindex][0].dc1_name[0]);
          }
          break;
        case 56:  // SU_HANA_01_DC2_NAME
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 5) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].dc2_name[0], tmp, strlen(tmp));
            debug_print("\nSU_HANA_01_DC2_NAME = %s\n",
                        hanasid[environmentindex][0].dc2_name[0]);
          }
          break;
        case 57:  // SU_HANA_02_DC1_NAME
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 5) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].dc1_name[1], tmp, strlen(tmp));
            debug_print("\nSU_HANA_02_DC1_NAME = %s\n",
                        hanasid[environmentindex][0].dc1_name[1]);
          }
          break;
        case 58:  // SU_HANA_01_DC2_NAME
          if (strlen(ConfigTable[i].result) > 2 &&
              strlen(ConfigTable[i].result) <= 5) {
            char *tmp = ConfigTable[i].result + 1;
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(hanasid[environmentindex][0].dc2_name[1], tmp, strlen(tmp));
            debug_print("\nSU_HANA_02_DC2_NAME = %s\n",
                        hanasid[environmentindex][0].dc2_name[1]);
          }
          break;
        case 59:  // SU_XHANA2_RELEASE
          strncpy(hanasid[environmentindex][0].xhana2_release,
                  ConfigTable[i].result, 4);
          debug_print("\nSU_XHANA2_RELEASE = %s\n",
                      hanasid[environmentindex][0].xhana2_release);
          break;
        case 60:  // SU_HANA_01_SAPREPO_VERSION_DC1
          strncpy(hanasid[environmentindex][0].saprepo_version_dc1[0],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_SAPREPO_VERSION_DC1 = %s\n",
                      hanasid[environmentindex][0].saprepo_version_dc1[0]);
          break;
        case 61:  // SU_HANA_01_SAPREPO_VERSION_DC2
          strncpy(hanasid[environmentindex][1].saprepo_version_dc2[0],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_SAPREPO_VERSION_DC2 = %s\n",
                      hanasid[environmentindex][1].saprepo_version_dc2[0]);
          break;
        case 62:  // SU_HANA_02_SAPREPO_VERSION_DC1
          strncpy(hanasid[environmentindex][0].saprepo_version_dc1[1],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_02_SAPREPO_VERSION_DC1 = %s\n",
                      hanasid[environmentindex][0].saprepo_version_dc1[1]);
          break;
        case 63:  // SU_HANA_02_SAPREPO_VERSION_DC2
          strncpy(hanasid[environmentindex][1].saprepo_version_dc2[1],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_02_SAPREPO_VERSION_DC2 = %s\n",
                      hanasid[environmentindex][1].saprepo_version_dc2[1]);
          break;
        case 64:  // SU_HANA_01_NAS_SVMS
          line = split_string(ConfigTable[i].result, ' ');

          for (int i = 0; i < 2; i++) {
            if (strlen(line[i]) > 2 &&
                strlen(line[i]) <= ConfigTable[i].maxlength) {
              char *tmp = line[i] + 1;
              tmp[strlen(tmp) - 1] = '\0';
              strncpy(hanasid[environmentindex][i].nas_svms[0], tmp,
                      strlen(tmp));
            }
          }
          debug_print("\nSU_HANA_01_NAS_SVMS DC1= %s   DC2=%s\n",
                      hanasid[environmentindex][0].nas_svms[0],
                      hanasid[environmentindex][1].nas_svms[0]);
          break;
        case 65:  // SU_HANA_02_NAS_SVMS
          line = split_string(ConfigTable[i].result, ' ');

          for (int i = 0; i < 2; i++) {
            if (strlen(line[i]) > 2 &&
                strlen(line[i]) <= ConfigTable[i].maxlength) {
              char *tmp = line[i] + 1;
              tmp[strlen(tmp) - 1] = '\0';
              strncpy(hanasid[environmentindex][i].nas_svms[1], tmp,
                      strlen(tmp));
            }
          }
          debug_print("\nSU_HANA_02_NAS_SVMS DC1= %s   DC2=%s\n",
                      hanasid[environmentindex][0].nas_svms[1],
                      hanasid[environmentindex][1].nas_svms[1]);
          break;
        case 66:  // SU_HANA_01_NAS_PORTS
          line = split_string(ConfigTable[i].result, ' ');

          for (int i = 0; i < 2; i++) {
            if (strlen(line[i]) == 1) {
              strncpy(hanasid[environmentindex][i].nas_ports[0], line[i],
                      strlen(line[i]));
            }
          }
          debug_print("\nSU_HANA_01_NAS_PORTS DC1=%s   DC2=%s\n",
                      hanasid[environmentindex][0].nas_ports[0],
                      hanasid[environmentindex][1].nas_ports[0]);
          break;
        case 67:  // SU_HANA_02_NAS_PORTS
          line = split_string(ConfigTable[i].result, ' ');

          for (int i = 0; i < 2; i++) {
            if (strlen(line[i]) == 1) {
              strncpy(hanasid[environmentindex][i].nas_ports[1], line[i],
                      strlen(line[i]));
            }
          }
          debug_print("\nSU_HANA_02_NAS_PORTS DC1=%s   DC2=%s\n",
                      hanasid[environmentindex][0].nas_ports[1],
                      hanasid[environmentindex][1].nas_ports[1]);
          break;
        case 68:  // SU_HANA_01_NAS_NUMSP
          if (ConfigTable[i].result != NULL) {
            int tmp = atoi(ConfigTable[i].result);
            if (tmp >= 1 && tmp <= 9) {
              hanasid[environmentindex][0].num_numsp[0] = tmp;
            } else {
              hanasid[environmentindex][0].num_numsp[0] = -1;
            }
          } else {
            hanasid[environmentindex][0].num_numsp[0] = -1;
          }

          debug_print("\nSU_HANA_01_NAS_NUMSP = %d\n",
                      hanasid[environmentindex][0].num_numsp[0]);

          FREE(line);
          break;
        case 69:  // SU_HANA_02_NAS_NUMSP
          if (ConfigTable[i].result != NULL) {
            int tmp = atoi(ConfigTable[i].result);
            if (tmp >= 1 && tmp <= 9) {
              hanasid[environmentindex][0].num_numsp[1] = tmp;
            } else {
              hanasid[environmentindex][0].num_numsp[1] = -1;
            }
          } else {
            hanasid[environmentindex][0].num_numsp[1] = -1;
          }

          debug_print("\nSU_HANA_02_NAS_NUMSP = %d\n",
                      hanasid[environmentindex][0].num_numsp[1]);

          FREE(line);
          break;
        case 70:  // SU_HANA_01_NAS_COMMENT
          strncpy(hanasid[environmentindex][0].nas_comment[0],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_NAS_COMMENT = %s\n",
                      hanasid[environmentindex][0].nas_comment[0]);
          break;
        case 71:  // SU_HANA_02_NAS_COMMENT
          strncpy(hanasid[environmentindex][0].nas_comment[1],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_02_NAS_COMMENT = %s\n",
                      hanasid[environmentindex][0].nas_comment[1]);
          break;
        case 72:  // SU_HANA_01_VIRTUAL_HOST_01
          strncpy(pmkhana[environmentindex].virtual_hostname[0],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_VIRTUAL_HOST_01 = %s\n",
                      pmkhana[environmentindex].virtual_hostname[0]);
          break;
        case 73:  // SU_HANA_01_VIRTUAL_IP_01
          strncpy(pmkhana[environmentindex].virtual_ip[0],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_VIRTUAL_IP_01 = %s\n",
                      pmkhana[environmentindex].virtual_ip[0]);
          break;
        case 74:  // SU_HANA_01_VIRTUAL_GW_01
          strncpy(pmkhana[environmentindex].virtual_gw[0],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_VIRTUAL_GW_01 = %s\n",
                      pmkhana[environmentindex].virtual_gw[0]);
          break;
        case 75:  // SU_HANA_01_VIRTUAL_NETMASK_01
          pmkhana[environmentindex].virtual_netmask[0] =
              atoi(ConfigTable[i].result);
          debug_print("\nSU_HANA_01_VIRTUAL_NETMASK_01 = %d\n",
                      pmkhana[environmentindex].virtual_netmask[0]);
          break;
        case 76:  // SU_HANA_01_VIRTUAL_VLAN_01
          pmkhana[environmentindex].virtual_vlan[0] =
              atoi(ConfigTable[i].result);
          debug_print("\nSU_HANA_01_VIRTUAL_VLAN_01 = %d\n",
                      pmkhana[environmentindex].virtual_vlan[0]);
          break;

        case 77:  // SU_HANA_01_VIRTUAL_HOST_02
          strncpy(pmkhana[environmentindex].virtual_hostname[1],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_VIRTUAL_HOST_02 = %s\n",
                      pmkhana[environmentindex].virtual_hostname[1]);
          break;
        case 78:  // SU_HANA_01_VIRTUAL_IP_02
          strncpy(pmkhana[environmentindex].virtual_ip[1],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_VIRTUAL_IP_02 = %s\n",
                      pmkhana[environmentindex].virtual_ip[1]);
          break;
        case 79:  // SU_HANA_01_VIRTUAL_GW_02
          strncpy(pmkhana[environmentindex].virtual_gw[1],
                  ConfigTable[i].result, ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_01_VIRTUAL_GW_02 = %s\n",
                      pmkhana[environmentindex].virtual_gw[1]);
          break;
        case 80:  // SU_HANA_01_VIRTUAL_NETMASK_02
          pmkhana[environmentindex].virtual_netmask[1] =
              atoi(ConfigTable[i].result);
          debug_print("\nSU_HANA_01_VIRTUAL_NETMASK_02 = %d\n",
                      pmkhana[environmentindex].virtual_netmask[1]);
          break;
        case 81:  // SU_HANA_01_VIRTUAL_VLAN_02
          pmkhana[environmentindex].virtual_vlan[1] =
              atoi(ConfigTable[i].result);
          debug_print("\nSU_HANA_01_VIRTUAL_VLAN_02 = %d\n",
                      pmkhana[environmentindex].virtual_vlan[1]);
          break;
        case 82:  // SU_HANA_01_PACE_NODES
          line = split_string(ConfigTable[i].result, ' ');
          if (line[0] != NULL) {
            char *tmp = line[0] + 1;
            tmp[strlen(tmp)] = '\0';
            strncpy(pmkhana[environmentindex].pace_nodes[0], tmp, strlen(tmp));
          }
          if (line[1] != NULL) {
            char *tmp = line[1];
            tmp[strlen(tmp) - 1] = '\0';
            strncpy(pmkhana[environmentindex].pace_nodes[1], tmp, strlen(tmp));
          }
          debug_print("\SU_HANA_01_PACE_NODES DC1=%s   DC2=%s\n",
                      pmkhana[environmentindex].pace_nodes[0],
                      pmkhana[environmentindex].pace_nodes[1]);
          break;
        case 83:  // SU_HANA_01_PACE_ISCSI_TARGETS
          line = split_string(ConfigTable[i].result, ' ');
          for (int i = 0; i < 3; i++) {
            if (strlen(line[i]) != 0) {
              strncpy(pmkhana[environmentindex].iscsi_target[i], line[i],
                      strlen(line[i]));
            }
          }
          debug_print("\nSU_HANA_01_PACE_ISCSI_TARGETS=%s  %s  %s   \n",
                      pmkhana[environmentindex].iscsi_target[0],
                      pmkhana[environmentindex].iscsi_target[1],
                      pmkhana[environmentindex].iscsi_target[2]);

          break;
        case 84:  // SU_HANA_IRMC_USER
          strncpy(pmkhana[environmentindex].irmc_user, ConfigTable[i].result,
                  ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_IRMC_USER = %s\n",
                      pmkhana[environmentindex].irmc_user);
          break;
        case 85:  // SU_HANA_IRMC_PWD
          strncpy(pmkhana[environmentindex].irmc_pwd, ConfigTable[i].result,
                  ConfigTable[i].maxlength);
          debug_print("\nSU_HANA_IRMC_PWD = %s\n",
                      pmkhana[environmentindex].irmc_pwd);
          break;
      }  // end of switch
    }    // enf of if check value is not null
  }      // end of for
  return EXIT_SUCCESS;
}

/*
 Read single ScaleUp Configuration File
*/

int read_su_file(char *filename, int environmentindex) {
  FILE *fd;
  char *filecontent;
  size_t filesize;

  printf("\n=========================================================");
  printf("\nRead file %s", filename);
  printf("\n=========================================================\n");

  fd = fopen(filename, "rb");
  if (fd == NULL) {
    fprintf(stderr, "Could not open %s\n", filename);
    return ERROR;
  }

  /* get filesize */
  fseek(fd, 0, SEEK_END);
  filesize = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  /* reserve memory */
  filecontent = (char *)malloc(filesize * sizeof(char));
  if (filecontent == NULL) {
    fprintf(stderr, "Could not allocate %ld Bytes.\n", filesize * sizeof(char));
    fclose(fd);
  }

  fread(filecontent, 1, filesize, fd);
  fclose(fd);
  // debug_print("Dateiinhalt:\n%s\n", filecontent);

  /* Scan filecontent */
  size_t n = sizeof(ConfigTable) / sizeof(ConfigTable[0]);
  for (size_t i = 0; i < n; i++) {
    ConfigTable[i].result =
        (char *)malloc((ConfigTable[i].maxlength + 1) * sizeof(char));
    /* debug_print("Parameter: %s    Regex: %s\n",
       ConfigTable[i].parametername, ConfigTable[i].regexeccode);
    */
    ConfigTable[i].result =
        find_matches(ConfigTable[i].regexeccode, filecontent);
    // debug_print("ConfigTable[%d].result = %s\n", i, ConfigTable[i].result);
  }
  free(filecontent);

  // debug
  /*
  for (size_t i = 0; i < n; i++) {
    debug_print("%s  --  %s\n", ConfigTable[i].parametername,
                ConfigTable[i].result);
  }
  */

  get_values(n, environmentindex, 2);  // 2 = ScaleUp, max. 2 Hosts

  return EXIT_SUCCESS;
}

int readconf_su(char *filedir) {
  Stack_t *filestack;
  int environmentindex = 0;

  filestack = get_files_in_confdir(filedir);

  if (isEmpty(filestack)) {
    printf("Stack is empty!\n");
    return EXIT_FAILURE;
  }

  Stack_t *current = filestack;
  while (current != NULL) {
    printf("\nRead %s as environmentindex %d", current->Configfile.filename,
           environmentindex);
    read_su_file(current->Configfile.filename, environmentindex);
    current = current->next;
    environmentindex++;
  }
  printf("\n");

  return EXIT_SUCCESS;
}