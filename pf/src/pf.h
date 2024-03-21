#ifndef PF_H_ /* Include guard */
#define PF_H_

#include <stdbool.h>
// #include <sys/types.h>
#include <stdint.h>

// 1. Abfrage Szenario Single DC oder Replikation ? SO oder SU wenn SU,
//     dann Single SID oder MCOS Toolserver ISCSI Server

#define HOSTNAME_LENGTH 14
#define MAX_ENVIRONMENTS 10
#define MAX_HOST_EACH_HANASYSTEM 12
#define MAX_SID_PER_ENVIRONMENT 6
#define SIDLENGTH 4   // SID Length
#define MACLENGTH 17  // MAC Length incl. colon
#define OK 0
#define ERROR 1
#define IP_MIN_LENGTH 7
#define IP_MAX_LENGTH 15
#define NET_INFOMATION 61  // used in readconf
#define TEXTLENGTH 24      // used in readconf

#define DEBUG 1

#define debug_print(fmt, ...)                                           \
  do {                                                                  \
    if (DEBUG)                                                          \
      fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, \
              __VA_ARGS__);                                             \
  } while (0)

/* get custom string
   https://suchprogramming.com/c-strings-and-standard-input/ */
typedef struct {
  ssize_t length;
  char *string;
  size_t buffer_size;
} CustomString;

/*
  SID Informationen
*/

typedef struct sidtype {
  char sid[SIDLENGTH];
  int installation_number;
  int allocation_limit;
  bool preload;
  int uid_sidadm;
  int uid_sapadm;
  int gid_sidshm;
  int gid_sapsys;
  int uid_saprouter;
  char nodes_dc1[MAX_SID_PER_ENVIRONMENT][TEXTLENGTH];
  char nodes_dc2[MAX_SID_PER_ENVIRONMENT][TEXTLENGTH];
  char nas_svms[MAX_SID_PER_ENVIRONMENT][TEXTLENGTH];
  char nas_ports[MAX_SID_PER_ENVIRONMENT][2];
  int num_numsp[MAX_SID_PER_ENVIRONMENT];
  char nas_comment[MAX_SID_PER_ENVIRONMENT][TEXTLENGTH];
  char dc1_name[MAX_SID_PER_ENVIRONMENT][SIDLENGTH];
  char dc2_name[MAX_SID_PER_ENVIRONMENT][SIDLENGTH];
  char xhana2_release[4];
  char saprepo_version_dc1[MAX_SID_PER_ENVIRONMENT][TEXTLENGTH];
  char saprepo_version_dc2[MAX_SID_PER_ENVIRONMENT][TEXTLENGTH];
  bool systemReplication;
} SIDTYPE;

/*
 Netwerkinformationen
 */
typedef struct networktype {
  char network_name[7];
  char network_host_ip[15];
  char network_ip[15];
  char network_gw[15];
  uint8_t network_netmask;
  uint16_t network_vlanid;
  uint16_t network_mtu;
} NETWORKTYPE;

typedef struct virthanasystemtype {
  char virtual_hostname[HOSTNAME_LENGTH];
  NETWORKTYPE network_ips;
  NETWORKTYPE network_se;
  NETWORKTYPE network_hnr;
  NETWORKTYPE network_client_sid;
} VIRTHANASYSTEMTYPE;

/*
HOST Informationen
*/
typedef struct hanasystemtype {
  char physical_hostname[HOSTNAME_LENGTH];
  // char virtual_hostname[HOSTNAME_LENGTH + 1];
  VIRTHANASYSTEMTYPE vhostname[MAX_SID_PER_ENVIRONMENT];
  char mac_address1[17];
  char mac_address2[17];
  NETWORKTYPE network_adm;
  NETWORKTYPE network_client;
  NETWORKTYPE network_st;
  NETWORKTYPE network_cr1;
  NETWORKTYPE network_cr2;
  NETWORKTYPE network_pcm;
  NETWORKTYPE network_bak;
  NETWORKTYPE network_hni;
} HANASYSTEMTYPE;

typedef struct pmkhanatype {
  char virtual_hostname[MAX_SID_PER_ENVIRONMENT][HOSTNAME_LENGTH];
  char virtual_ip[MAX_SID_PER_ENVIRONMENT][IP_MAX_LENGTH];
  char virtual_gw[MAX_SID_PER_ENVIRONMENT][IP_MAX_LENGTH];
  int virtual_netmask[MAX_SID_PER_ENVIRONMENT];
  int virtual_vlan[MAX_SID_PER_ENVIRONMENT];
  char pace_nodes[MAX_SID_PER_ENVIRONMENT][HOSTNAME_LENGTH];
  char iscsi_target[3][TEXTLENGTH];
  char irmc_user[TEXTLENGTH];
  char irmc_pwd[TEXTLENGTH];
} PMKHANATYPE;

/* interactive.c */
void get_sid_list(void);
void get_system_data(int max_hana_systems);
void get_systemtype_choice(void);
int get_number_systems(void);
int get_number_sids(void);
int interactive(void);

// readconf_su
int readconf_su(char *filename, int environmentindex);
#endif