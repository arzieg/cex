#ifndef PF_H_ /* Include guard */
#define PF_H_

#include <stdbool.h>
// 1. Abfrage Szenario Single DC oder Replikation ? SO oder SU wenn SU,
//     dann Single SID oder MCOS Toolserver ISCSI Server

#define hostname_length 13

typedef struct sid {
  char *sid[3];
  bool mcos;
  bool systemReplication;
} SID;

typedef struct hanasystem {
  char *physical_hostname[hostname_length];
  char *virtual_hostname[hostname_length];
  char *mac_address[17];
  SID sid;
} HANASYSTEM;

/* interactive.c */
void get_sid_list(void);
int get_systemtype_choice(void);
int interactive(void);

#endif
