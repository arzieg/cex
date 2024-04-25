#ifndef PF_HELPER_ /* Include guard */
#define PF_HELPER_

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "pf.h"

/*
 for readconf.c. Idea is to setup a table with parametername, regexec-code and
 result of regexec.
*/
// Parameter | regexec | result
struct ConfigTableArray {
  char *parametername;
  char *regexeccode;
  size_t maxlength;  // max length of string
  size_t index;
  char *result;
};

void CustomString_free(CustomString *target);

// CustomString *custom_getline(FILE *stream, int minchars, int maxchars,
//                              int stringfunction);
bool get_yesno_status(char *text, FILE *stream);

void to_upper_case(char *line, int n);
void to_lower_case(char *line, int n);
void clear_stdin(void);
size_t get_systeminformation_int(char *text, size_t min, size_t max);

CustomString *custom_getline(FILE *stream, int minchars, int maxchars,
                             char *stringfunction);
bool CustomString_check_regex(CustomString *target, char *customregex);
char **split_string(char *input, char delimiter);
// readconf_su
int readconf_su(char *filedir);
int read_su_file(char *filename, int environmentindex);
char *find_matches(const char *pattern, const char *text);

#endif