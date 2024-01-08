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
  int installation_number;
  int uid_sidadm;
  int uid_sapadm;
  int gid_sidshm;
  int gid_sapsys;
  char nodes_dc1[MAX_HOST_EACH_HANASYSTEM][20];
  char nodes_dc2[MAX_HOST_EACH_HANASYSTEM][20];
  char nas_ports[4];
  int num_numsp;
  char nas_comment;
  char dc1_name[3];
  char dc2_name[3];
  char xhana2_release[3];
  char saprepo_version_dc1[24];
  char saprepo_version_dc2[24];
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
