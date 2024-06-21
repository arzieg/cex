#include "sshconnect.h"

#include <errno.h>
#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// extern int errno;

void printerrorcode(int errcode) {
  fprintf(stderr, "Error in strtoul, got errorcode %d: %s\n", errcode,
          strerror(errcode));
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "usage: sshconnect hostname port\n");
    exit(EXIT_FAILURE);
  }

  ssh_session my_ssh_session;
  int verbosity = SSH_LOG_PROTOCOL;
  size_t port;
  char *tmp;
  char *hostname = argv[1];

  // get port number
  errno = 0;
  port = strtoul(argv[2], &tmp, 10);
  if (errno != 0) printerrorcode(errno);

  if (port < 1 || port > 65535) {
    fprintf(stderr, "Error port out of range\n");
    exit(EXIT_FAILURE);
  }

  errno = 0;
  my_ssh_session = ssh_new();
  if (errno != 0) printerrorcode(errno);

  ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, hostname);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
  ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

  int rc = ssh_connect(my_ssh_session);
  if (rc != SSH_OK) {
    fprintf(stderr, "Error connecting to %s: %s\n", hostname,
            ssh_get_error(my_ssh_session));
    exit(EXIT_FAILURE);
  }

  ssh_free(my_ssh_session);
  exit(EXIT_SUCCESS);
}