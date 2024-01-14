#ifndef PF_HELPER_ /* Include guard */
#define PF_HELPER_

#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>

#include "pf.h"

enum stringfunction { ISALPHANUMERIC, ISALPHAORCOLON };

void CustomString_free(CustomString *target);
bool CustomString_isalphaORcolon(CustomString *target);

CustomString *custom_getline(FILE *stream, int minchars, int maxchars,
                             int stringfunction);
bool get_yesno_status(char *text, FILE *stream);
void toUpperCase(char *line, int n);
#endif