// implicit declaration of function getline
#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"
#include "pfhelper.h"

#define REGEXMAC "\\((([a-fA-F0-9]{2}[:-]){5}([a-fA-F0-9]{2}))\\)"
#define REGEXSTR "\\(([^()]*)\\)"

struct ConfigTableArray ConfigTable[] = {
    {.parametername = "SU_NET_MACS1_DC2",
     .regexeccode = "SU_NET_MACS1_DC2=" REGEXSTR "",
     .maxlength = MACLENGTH},
    {.parametername = "SU_NET_MACS1_DC1",
     .regexeccode = "SU_NET_MACS1_DC1=" REGEXSTR "",
     .maxlength = MACLENGTH},
    {.parametername = "SU_NET_MACS2_DC2",
     .regexeccode = "SU_NET_MACS2_DC2=" REGEXSTR "",
     .maxlength = MACLENGTH},
    {.parametername = "SU_NET_MACS2_DC1",
     .regexeccode = "SU_NET_MACS2_DC1=" REGEXSTR "",
     .maxlength = MACLENGTH},
    {.parametername = "SU_NET_HOSTS_DC1",
     .regexeccode = "SU_NET_HOSTS_DC1=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH},
    {.parametername = "SU_NET_HOSTS_DC2",
     .regexeccode = "SU_NET_HOSTS_DC2=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH},
    {.parametername = "SU_NET_HANA_01_HOSTS_DC1",
     .regexeccode = "SU_NET_HANA_01_HOSTS_DC1=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH},
    {.parametername = "SU_NET_HANA_01_HOSTS_DC2",
     .regexeccode = "SU_NET_HANA_01_HOSTS_DC2=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH},
    {.parametername = "SU_NET_HANA_02_HOSTS_DC1",
     .regexeccode = "SU_NET_HANA_02_HOSTS_DC1=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH},
    {.parametername = "SU_NET_HANA_02_HOSTS_DC2",
     .regexeccode = "SU_NET_HANA_02_HOSTS_DC2=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH},
    {.parametername = "SU_NET_HANA_01_IPS_DC1",
     .regexeccode = "SU_NET_HANA_01_IPS_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_IPS_DC2",
     .regexeccode = "SU_NET_HANA_01_IPS_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_IPS_DC1",
     .regexeccode = "SU_NET_HANA_02_IPS_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_IPS_DC2",
     .regexeccode = "SU_NET_HANA_02_IPS_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_ADM_DC1",
     .regexeccode = "SU_NET_ADM_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_ADM_DC2",
     .regexeccode = "SU_NET_ADM_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_CLIENT_DC1",
     .regexeccode = "SU_NET_CLIENT_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_CLIENT_DC2",
     .regexeccode = "SU_NET_CLIENT_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_ST_DC1",
     .regexeccode = "SU_NET_ST_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_ST_DC2",
     .regexeccode = "SU_NET_ST_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_CR1_DC1",
     .regexeccode = "SU_NET_CR1_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_CR1_DC2",
     .regexeccode = "SU_NET_CR1_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_CR2_DC1",
     .regexeccode = "SU_NET_CR2_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_CR2_DC2",
     .regexeccode = "SU_NET_CR2_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_PCM_DC1",
     .regexeccode = "SU_NET_PCM_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_PCM_DC2",
     .regexeccode = "SU_NET_PCM_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_BAK_DC1",
     .regexeccode = "SU_NET_BAK_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_BAK_DC2",
     .regexeccode = "SU_NET_BAK_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_SE_DC1",
     .regexeccode = "SU_NET_HANA_01_SE_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_SE_DC2",
     .regexeccode = "SU_NET_HANA_01_SE_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_CLIENT_DC1",
     .regexeccode = "SU_NET_HANA_01_CLIENT_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_CLIENT_DC2",
     .regexeccode = "SU_NET_HANA_01_CLIENT_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_SE_DC1",
     .regexeccode = "SU_NET_HANA_02_SE_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_SE_DC2",
     .regexeccode = "SU_NET_HANA_02_SE_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_CLIENT_DC1",
     .regexeccode = "SU_NET_HANA_02_CLIENT_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_CLIENT_DC2",
     .regexeccode = "SU_NET_HANA_02_CLIENT_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_HNR_DC1",
     .regexeccode = "SU_NET_HANA_01_HNR_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_01_HNR_DC2",
     .regexeccode = "SU_NET_HANA_01_HNR_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_HNR_DC1",
     .regexeccode = "SU_NET_HANA_02_HNR_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_HNR_DC2",
     .regexeccode = "SU_NET_HANA_02_HNR_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_HNR_DC1",
     .regexeccode = "SU_NET_HANA_02_HNR_DC1=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_NET_HANA_02_HNR_DC2",
     .regexeccode = "SU_NET_HANA_02_HNR_DC2=" REGEXSTR "",
     .maxlength = NET_INFOMATION},
    {.parametername = "SU_VIRTUAL",
     .regexeccode = "SU_VIRTUAL=" REGEXSTR "",
     .maxlength = TEXTLENGTH},
    {.parametername = "SU_HANA_SID",
     .regexeccode = "SU_HANA_SID=" REGEXSTR "",
     .maxlength =
         SIDLENGTH * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_HANA_INR",
     .regexeccode = "SU_HANA_INR=" REGEXSTR "",
     .maxlength =
         SIDLENGTH * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_HANA_ALLOC_LIMIT_GB",
     .regexeccode = "SU_HANA_ALLOC_LIMIT_GB=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_HANA_PRELOAD_TABLES",
     .regexeccode = "SU_HANA_PRELOAD_TABLES=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_UID_SIDADM",
     .regexeccode = "SU_UID_SIDADM=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_UID_SAPADM",
     .regexeccode = "SU_UID_SAPADM=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_GID_SIDSHM",
     .regexeccode = "SU_GID_SIDSHM=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_GID_SAPSYS",
     .regexeccode = "SU_GID_SAPSYS=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_UID_SAPROUTER",
     .regexeccode = "SU_UID_SAPROUTER=" REGEXSTR "",
     .maxlength = 5 * MAX_SID_PER_ENVIRONMENT + MAX_SID_PER_ENVIRONMENT},
    {.parametername = "SU_HANA_01_NODES_DC1",
     .regexeccode = "SU_HANA_01_NODES_DC1=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH + 8},
    {.parametername = "SU_HANA_01_NODES_DC2",
     .regexeccode = "SU_HANA_01_NODES_DC1=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH + 8},
    {.parametername = "SU_HANA_02_NODES_DC1",
     .regexeccode = "SU_HANA_02_NODES_DC1=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH + 8},
    {.parametername = "SU_HANA_02_NODES_DC2",
     .regexeccode = "SU_HANA_02_NODES_DC2=" REGEXSTR "",
     .maxlength = HOSTNAME_LENGTH + 8},
    {.parametername = "SU_HANA_01_DC1_NAME",
     .regexeccode = "SU_HANA_01_DC1_NAME=" REGEXSTR "",
     .maxlength = 4},
    {.parametername = "SU_HANA_01_DC2_NAME",
     .regexeccode = "SU_HANA_01_DC2_NAME=" REGEXSTR "",
     .maxlength = 4},
    {.parametername = "SU_HANA_02_DC1_NAME",
     .regexeccode = "SU_HANA_02_DC1_NAME=" REGEXSTR "",
     .maxlength = 4},
    {.parametername = "SU_HANA_02_DC2_NAME",
     .regexeccode = "SU_HANA_02_DC2_NAME=" REGEXSTR "",
     .maxlength = 4},
    {.parametername = "SU_XHANA2_RELEASE",
     .regexeccode = "SU_XHANA2_RELEASE=" REGEXSTR "",
     .maxlength = TEXTLENGTH},

};

/*

SU_HANA_01_SAPREPO_VERSION_DC1="BWoH_Release_16"
SU_HANA_01_SAPREPO_VERSION_DC2="BWoH_Release_17"
SU_HANA_02_SAPREPO_VERSION_DC1="BWoH_Release_15"
SU_HANA_02_SAPREPO_VERSION_DC2="BWoH_Release_15"
SU_HANA_01_NAS_SVMS=("dc1-hana-n" "dc2-hana-n")
SU_HANA_02_NAS_SVMS=("dc1-hana-n" "dc2-hana-n")
SU_HANA_01_NAS_PORTS=(a b)
SU_HANA_02_NAS_PORTS=(b a)
SU_HANA_01_NAS_NUMSP=1         # Number of storage partitions (data+log volumes)
SU_HANA_02_NAS_NUMSP=1         # Number of storage partitions (data+log volumes)
SU_HANA_01_NAS_COMMENT=        # if empty /etc/fstab entries are active,
otherwise started with a comment SU_HANA_02_NAS_COMMENT=        # if empty
/etc/fstab entries are active, otherwise started with a comment SU_NAS_DC1="dc1"
SU_NAS_DC2="dc2"


*/

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
    cursor = malloc((len + 1) * sizeof(char));
    strncpy(cursor, text + off, len);
    cursor[len + 1] = '\0';
    // debug_print("\n found = %s\n", cursor);
  }

  regfree(&regexCompiled);

  return (cursor != NULL) ? cursor : NULL;
}

int readconf(char *filename) {
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
    debug_print("Parameter: %s    Regex: %s\n", ConfigTable[i].parametername,
                ConfigTable[i].regexeccode);
    ConfigTable[i].result =
        find_matches(ConfigTable[i].regexeccode, filecontent);
    debug_print("ConfigTable[%d].result = %s\n", i, ConfigTable[i].result);
  }

  // Speicher freigeben
  free(filecontent);

  return EXIT_SUCCESS;
}
