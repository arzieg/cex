// implicit declaration of function getline
#define _GNU_SOURCE
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pf.h"
#include "pfhelper.h"

struct ConfigTableArray ConfigTable[] = {
    {.parametername = "SU_NET_MACS1_DC2",
     .regexeccode =
         "SU_NET_MACS1_DC2=\\((([a-fA-F0-9]{2}[:-]){5}([a-fA-F0-9]{2}))\\)"},
    {.parametername = "SU_NET_MACS1_DC1",
     .regexeccode =
         "SU_NET_MACS1_DC1=\\((([a-fA-F0-9]{2}[:-]){5}([a-fA-F0-9]{2}))\\)"},

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

  ConfigTable[0].result = (char *)malloc(50 * sizeof(char));
  ConfigTable[1].result = (char *)malloc(100 * sizeof(char));

  for (size_t i = 0; i < 2; i++) {
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
