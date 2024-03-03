#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct networktype {
  char network_name[7];
  char network_host_ip[15];
  char network_ip[15];
  char network_gw[15];
  uint8_t network_netmask;
  uint16_t network_vlanid;
  uint16_t network_mtu;
} NETWORKTYPE;

typedef struct hanasystemtype {
  NETWORKTYPE network;
} HANASYSTEMTYPE;

/*
  Split String in Single Token
  Used in readconf_su.c
*/
char **split_string(char *input, char delimiter) {
  char **tokens = NULL;
  char *token;
  char *rest = strdup(input);
  int count = 0;

  // Zähle die Anzahl der Tokens
  while ((token = strsep(&rest, &delimiter)) != NULL) {
    count++;
  }

  // Alloziere Speicher für das Token-Array
  tokens = (char **)malloc(sizeof(char *) * count);
  if (!tokens) {
    perror("Fehler bei der Speicherzuweisung");
    exit(1);
  }

  // Setze den Rest-Pointer zurück
  rest = strdup(input);
  int i = 0;

  // Fülle das Token-Array
  while ((token = strsep(&rest, &delimiter)) != NULL) {
    printf("\nToken: %s", token);
    tokens[i] = strdup(token);
    i++;
  }

  return tokens;
}

/*
  split line and assign to variable
*/

void split_line_and_assign_to_variable(char *line,
                                       HANASYSTEMTYPE *phanasystem) {
  printf("\nIn Funktion split line and assign to variable\n");
  printf("line = %s", line);

  char **token_array = split_string(line, ' ');

  printf("\nnetwork_name = %p", phanasystem->network.network_name);
  printf("\nnetwork_host_ip %p", phanasystem->network.network_host_ip);
  printf("\nnetwork_ip %p", phanasystem->network.network_ip);
  printf("\nnetwork_gw %p", phanasystem->network.network_gw);
  printf("\nnetwork_netmask %p", &phanasystem->network.network_netmask);
  printf("\nnetwork_vlanid %p", &phanasystem->network.network_vlanid);
  printf("\nnetwork_mtu %p", &phanasystem->network.network_mtu);

  if (token_array) {
    for (int i = 0; token_array[i] != NULL; i++) {
      printf("\nWort %d: [%s]", i, token_array[i]);
      switch (i) {
        case 0:
          strncpy(phanasystem->network.network_name, "IPS", 4);
          strncpy(phanasystem->network.network_ip, token_array[i], 15);
          break;
        case 1:
          strncpy(phanasystem->network.network_gw, token_array[i], 15);
          break;
        case 2:
          phanasystem->network.network_netmask = (uint8_t)atoi(token_array[i]);
          break;
        case 3:
          phanasystem->network.network_vlanid = (uint16_t)atoi(token_array[i]);
          break;
        case 4:
          phanasystem->network.network_mtu = (uint16_t)atoi(token_array[i]);
          break;

        case 5:
          strncpy(phanasystem->network.network_host_ip, token_array[i], 15);
          break;
      }
      free(token_array[i]);
    }
    free(token_array);
  }
}

int main() {
  HANASYSTEMTYPE hanasystem[1][1];

  // HANASYSTEMTYPE *SU_NET_HANA_01_SE_DC1[7];
  HANASYSTEMTYPE *phanasystem;

  /*
  HANASYSTEMTYPE *phanasystem[0] = {
      &hanasystem[0][0].network_se.network_name,
      &hanasystem[0][0].network_se.network_host_ip,
      &hanasystem[0][0].network_se.network_ip,
      &hanasystem[0][0].network_se.network_gw,
      &hanasystem[0][0].network_se.network_netmask,
      &hanasystem[0][0].network_se.network_vlanid,
      &hanasystem[0][0].network_se.network_mtu};
*/
  char *line = "192.168.64.0 192.168.64.1 24 664 9000 192.168.64.107";

  phanasystem = &hanasystem[0][0];

  printf("\nnetwork_name = %p", phanasystem->network.network_name);
  printf("\nnetwork_host_ip %p", phanasystem->network.network_host_ip);
  printf("\nnetwork_ip %p", phanasystem->network.network_ip);
  printf("\nnetwork_gw %p", phanasystem->network.network_gw);
  printf("\nnetwork_netmask %p", &phanasystem->network.network_netmask);
  printf("\nnetwork_vlanid %p", &phanasystem->network.network_vlanid);
  printf("\nnetwork_mtu %p", &phanasystem->network.network_mtu);

  printf("\nline = %s", line);

  split_line_and_assign_to_variable(line, phanasystem);

  printf("\nnetwork_name = %s", phanasystem->network.network_name);
  printf("\nnetwork_host_ip %s", phanasystem->network.network_host_ip);
  printf("\nnetwork_ip %s", phanasystem->network.network_ip);
  printf("\nnetwork_gw %s", phanasystem->network.network_gw);
  printf("\nnetwork_netmask %d", phanasystem->network.network_netmask);
  printf("\nnetwork_vlanid %d", phanasystem->network.network_vlanid);
  printf("\nnetwork_mtu %d", phanasystem->network.network_mtu);

  return 0;
}