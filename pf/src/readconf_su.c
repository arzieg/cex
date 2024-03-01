// implicit declaration of function getline
#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"
#include "pfhelper.h"

#define REGEXMAC "\\((([a-fA-F0-9]{2}[:-]){5}([a-fA-F0-9]{2}))\\)"
#define REGEXBRACKETSTR "\\(([^()]*)\\)"
#define REGEXSTR "\"([^\"]*)\""

extern HANASYSTEMTYPE hanasystem[MAX_ENVIRONMENTS][MAX_HOST_EACH_HANASYSTEM];
extern SIDTYPE hanasid[MAX_ENVIRONMENTS][MAX_SID_PER_ENVIRONMENT];

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
    {.parametername = "SU_NET_HANA_02_HNR_DC1",
     .regexeccode = "SU_NET_HANA_02_HNR_DC1=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 41},
    {.parametername = "SU_NET_HANA_02_HNR_DC2",
     .regexeccode = "SU_NET_HANA_02_HNR_DC2=" REGEXBRACKETSTR "",
     .maxlength = NET_INFOMATION,
     .index = 42},
    {.parametername = "SU_VIRTUAL",
     .regexeccode = "SU_VIRTUAL=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 43},
    {.parametername = "SU_HANA_SID",
     .regexeccode = "SU_HANA_SID=" REGEXBRACKETSTR "",
     .maxlength = SIDLENGTH * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 44},
    {.parametername = "SU_HANA_INR",
     .regexeccode = "SU_HANA_INR=" REGEXBRACKETSTR "",
     .maxlength = SIDLENGTH * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 45},
    {.parametername = "SU_HANA_ALLOC_LIMIT_GB",
     .regexeccode = "SU_HANA_ALLOC_LIMIT_GB=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 46},
    {.parametername = "SU_HANA_PRELOAD_TABLES",
     .regexeccode = "SU_HANA_PRELOAD_TABLES=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 47},
    {.parametername = "SU_UID_SIDADM",
     .regexeccode = "SU_UID_SIDADM=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 48},
    {.parametername = "SU_UID_SAPADM",
     .regexeccode = "SU_UID_SAPADM=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 49},
    {.parametername = "SU_GID_SIDSHM",
     .regexeccode = "SU_GID_SIDSHM=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 50},
    {.parametername = "SU_GID_SAPSYS",
     .regexeccode = "SU_GID_SAPSYS=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 51},
    {.parametername = "SU_UID_SAPROUTER",
     .regexeccode = "SU_UID_SAPROUTER=" REGEXBRACKETSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT,
     .index = 52},
    {.parametername = "SU_HANA_01_NODES_DC1",
     .regexeccode = "SU_HANA_01_NODES_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 53},
    {.parametername = "SU_HANA_01_NODES_DC2",
     .regexeccode = "SU_HANA_01_NODES_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 54},
    {.parametername = "SU_HANA_02_NODES_DC1",
     .regexeccode = "SU_HANA_02_NODES_DC1=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 55},
    {.parametername = "SU_HANA_02_NODES_DC2",
     .regexeccode = "SU_HANA_02_NODES_DC2=" REGEXBRACKETSTR "",
     .maxlength = HOSTNAME_LENGTH + 8,
     .index = 56},
    {.parametername = "SU_HANA_01_DC1_NAME",
     .regexeccode = "SU_HANA_01_DC1_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 57},
    {.parametername = "SU_HANA_01_DC2_NAME",
     .regexeccode = "SU_HANA_01_DC2_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 58},
    {.parametername = "SU_HANA_02_DC1_NAME",
     .regexeccode = "SU_HANA_02_DC1_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 59},
    {.parametername = "SU_HANA_02_DC2_NAME",
     .regexeccode = "SU_HANA_02_DC2_NAME=" REGEXBRACKETSTR "",
     .maxlength = 4,
     .index = 60},
    {.parametername = "SU_XHANA2_RELEASE",
     .regexeccode = "SU_XHANA2_RELEASE=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 61},
    {.parametername = "SU_HANA_01_SAPREPO_VERSION_DC1",
     .regexeccode = "SU_HANA_01_SAPREPO_VERSION_DC1=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 62},
    {.parametername = "SU_HANA_01_SAPREPO_VERSION_DC2",
     .regexeccode = "SU_HANA_01_SAPREPO_VERSION_DC2=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 63},
    {.parametername = "SU_HANA_02_SAPREPO_VERSION_DC1",
     .regexeccode = "SU_HANA_02_SAPREPO_VERSION_DC1=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 64},
    {.parametername = "SU_HANA_02_SAPREPO_VERSION_DC2",
     .regexeccode = "SU_HANA_02_SAPREPO_VERSION_DC2=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 65},
    {.parametername = "SU_HANA_01_NAS_SVMS",
     .regexeccode = "SU_HANA_01_NAS_SVMS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 66},
    {.parametername = "SU_HANA_02_NAS_SVMS",
     .regexeccode = "SU_HANA_02_NAS_SVMS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 67},
    {.parametername = "SU_HANA_01_NAS_PORTS",
     .regexeccode = "SU_HANA_01_NAS_PORTS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 68},
    {.parametername = "SU_HANA_02_NAS_PORTS",
     .regexeccode = "SU_HANA_02_NAS_PORTS=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 69},
    {.parametername = "SU_HANA_01_NAS_NUMSP",
     .regexeccode = "SU_HANA_01_NAS_NUMSP=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 70},
    {.parametername = "SU_HANA_02_NAS_NUMSP",
     .regexeccode = "SU_HANA_02_NAS_NUMSP=" REGEXBRACKETSTR "",
     .maxlength = TEXTLENGTH,
     .index = 71},
    {.parametername = "SU_HANA_01_NAS_COMMENT",
     .regexeccode = "SU_HANA_01_NAS_COMMENT=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 72},
    {.parametername = "SU_NAS_DC1",
     .regexeccode = "SU_NAS_DC1=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 73},
    {.parametername = "SU_NAS_DC2",
     .regexeccode = "SU_NAS_DC2=" REGEXSTR "",
     .maxlength = TEXTLENGTH,
     .index = 74},

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

int get_values(int configtablecount, int environmentindex, int maxhost) {
  /*
  Durchlaufe eine Schleife 0-n von ConfigTable[i].parametername
  case
    xxx setze wert
      wenn wert noch eine zusammenhängende Zeile, dann aufteilen
  extern HANASYSTEMTYPE hanasystem[MAX_ENVIRONMENTS][MAX_HOST_EACH_HANASYSTEM];
  extern SIDTYPE hanasid[MAX_ENVIRONMENTS][MAX_SID_PER_ENVIRONMENT];
  */
  for (size_t i = 0; i < configtablecount; i++) {
    switch (ConfigTable[i].index) {
      case 1:  // SU_NET_MACS1_DC2
        strncpy(hanasystem[environmentindex][1].mac_address1,
                ConfigTable[i].result, 17);
        break;
      case 2:  // SU_NET_MACS1_DC1
        strncpy(hanasystem[environmentindex][0].mac_address1,
                ConfigTable[i].result, 17);
        break;
      case 3:  // SU_NET_MACS2_DC2
        strncpy(hanasystem[environmentindex][1].mac_address2,
                ConfigTable[i].result, 17);
        break;
      case 4:  // SU_NET_MACS2_DC1
        strncpy(hanasystem[environmentindex][0].mac_address2,
                ConfigTable[i].result, 17);
        break;
      case 5:  // SU_NET_HOSTS_DC1
        strncpy(hanasystem[environmentindex][0].physical_hostname,
                ConfigTable[i].result, HOSTNAME_LENGTH + 1);
        break;
      case 6:  // SU_NET_HOSTS_DC2
        strncpy(hanasystem[environmentindex][1].physical_hostname,
                ConfigTable[i].result, HOSTNAME_LENGTH + 1);
        break;
      case 7:  // SU_NET_HANA_01_HOSTS_DC1
        strncpy(hanasystem[environmentindex][0].vhostname[0].virtual_hostname,
                ConfigTable[i].result, HOSTNAME_LENGTH + 1);
        break;
      case 8:  // SU_NET_HANA_01_HOSTS_DC2
        strncpy(hanasystem[environmentindex][1].vhostname[0].virtual_hostname,
                ConfigTable[i].result, HOSTNAME_LENGTH + 1);
        break;
      case 9:  // SU_NET_HANA_02_HOSTS_DC1
        strncpy(hanasystem[environmentindex][0].vhostname[1].virtual_hostname,
                ConfigTable[i].result, HOSTNAME_LENGTH + 1);
        break;
      case 10:  // SU_NET_HANA_02_HOSTS_DC2
        strncpy(hanasystem[environmentindex][1].vhostname[1].virtual_hostname,
                ConfigTable[i].result, HOSTNAME_LENGTH + 1);
        break;

      default:
        break;
    }
  }
  return 0;
}

int readconf_su(char *filename, int environmentindex) {
  FILE *fd;
  char *filecontent;
  size_t filesize;

  fd = fopen(filename, "rb");
  if (fd == NULL) {
    fprintf(stderr, "Could not open %s\n", filename);
    return ERROR;
  }

  // get filesize
  fseek(fd, 0, SEEK_END);
  filesize = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  // reserve memory
  filecontent = (char *)malloc(filesize * sizeof(char));
  if (filecontent == NULL) {
    fprintf(stderr, "Could not allocate %ld Bytes.\n", filesize * sizeof(char));
    fclose(fd);
  }

  fread(filecontent, 1, filesize, fd);
  fclose(fd);

  // debug_print("Dateiinhalt:\n%s\n", filecontent);

  size_t n = sizeof(ConfigTable) / sizeof(ConfigTable[0]);
  for (size_t i = 0; i < n; i++) {
    ConfigTable[i].result =
        (char *)malloc((ConfigTable[i].maxlength + 1) * sizeof(char));
    /* debug_print("Parameter: %s    Regex: %s\n", ConfigTable[i].parametername,
                ConfigTable[i].regexeccode);
    */
    ConfigTable[i].result =
        find_matches(ConfigTable[i].regexeccode, filecontent);
    // debug_print("ConfigTable[%d].result = %s\n", i, ConfigTable[i].result);
  }
  free(filecontent);

  // debug
  for (size_t i = 0; i < n; i++) {
    debug_print("%s  --  %s\n", ConfigTable[i].parametername,
                ConfigTable[i].result);
  }

  get_values(n, environmentindex, 2);

  return EXIT_SUCCESS;
}
