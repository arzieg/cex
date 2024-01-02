#ifndef PF_H_ /* Include guard */
#define PF_H_

#include <stdbool.h>
// 1. Abfrage Szenario Single DC oder Replikation ? SO oder SU wenn SU,
//     dann Single SID oder MCOS Toolserver ISCSI Server

#define HOSTNAME_LENGTH 13
#define MAX_ENVIRONMENTS 10
#define MAX_HOST_EACH_HANASYSTEM 13
#define MAX_SID_PER_ENVIRONMENT 5
#define SIDLENGTH 4  // SID Length + 1 für CR
#define OK 0
#define ERROR 1

typedef struct sidtype {
  char sid[SIDLENGTH];
  bool mcos;
  bool systemReplication;
} SIDTYPE;

typedef struct hanasystemtype {
  char *physical_hostname[HOSTNAME_LENGTH];
  char *virtual_hostname[HOSTNAME_LENGTH];
  char *mac_address[17];
  SIDTYPE hanasid[MAX_SID_PER_ENVIRONMENT];
} HANASYSTEMTYPE;

/* interactive.c */
void get_sid_list(void);
int get_systemtype_choice(void);
int interactive(void);

#endif
