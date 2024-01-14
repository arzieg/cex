#ifndef PF_HELPER_ /* Include guard */
#define PF_HELPER_

#include <stdbool.h>
#include <sys/types.h>

#include "pf.h"

void CustomString_free(CustomString *target);
bool CustomString_isalphanumeric(CustomString *target);

CustomString *custom_getline(FILE *stream, int minchars, int maxchars);
bool get_yesno_status(char *text, FILE *stream);
void toUpperCase(char *line, int n);
#endif