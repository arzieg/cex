#ifndef PF_H_ /* Include guard */
#define PF_H_

#include <stdbool.h>
#include <sys/types.h>

// 1. Abfrage Szenario Single DC oder Replikation ? SO oder SU wenn SU,
//     dann Single SID oder MCOS Toolserver ISCSI Server

#define HOSTNAME_LENGTH 13
#define MAX_ENVIRONMENTS 10
#define MAX_HOST_EACH_HANASYSTEM 13
#define MAX_SID_PER_ENVIRONMENT 1
#define SIDLENGTH 3  // SID Length
#define OK 0
#define ERROR 1

/* get custom string
   https://suchprogramming.com/c-strings-and-standard-input/ */
typedef struct {
  ssize_t length;
  char *string;
  size_t buffer_size;
} CustomString;

typedef struct sidtype {
  char sid[SIDLENGTH + 1];
  bool systemReplication;
} SIDTYPE;

/*
typedef struct hanasidtype {
  SIDTYPE hanasid[MAX_SID_PER_ENVIRONMENT];
} HANASIDTYPE;
*/
typedef struct hanasystemtype {
  char *physical_hostname[HOSTNAME_LENGTH];
  char *virtual_hostname[HOSTNAME_LENGTH];
  char *mac_address[17];
} HANASYSTEMTYPE;

/* interactive.c */
void get_sid_list(void);
int get_systemtype_choice(void);
int interactive(void);

#endif
