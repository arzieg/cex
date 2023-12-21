#ifndef PF_H_ /* Include guard */
#define PF_H_

// 1. Abfrage Szenario Single DC oder Replikation ? SO oder SU wenn SU,
//     dann Single SID oder MCOS Toolserver ISCSI Server

#define hostname_length 13

typedef struct hanasystem {
  char *physical_hostname[hostname_length];
  char *virtual_hostname[hostname_length];
  char *mac_address[17];
} HANASYSTEM;

/* interactive.c */
int interactive(void);

#endif
