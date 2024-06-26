/*

functions
  verify_known_hosts (ok)
  authenticate_user
  doing_something
  connect_to_server

  main
   connnect_to_server
   authenticate_user
   verify_known_hosts
   doing_something

*/

#include "sshshell.h"

#include <errno.h>
#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int establish_shell_session(ssh_session session, ssh_channel *channel)
{
    int rc;

    *channel = ssh_channel_new(session);
    if (*channel == NULL)
        return SSH_ERROR;

    rc = ssh_channel_open_session(*channel);
    if (rc != SSH_OK)
    {
        ssh_channel_free(*channel);
        return rc;
    }

    return SSH_OK;
}

int shell_cmd(ssh_channel channel, const char *cmd)
{
    if (ssh_channel_open_session(channel) != SSH_OK)
    {
        fprintf(stderr, "ssh_channel_open_session() failed.\n");
        return 0;
    }

    if (ssh_channel_request_exec(channel, cmd) != SSH_OK)
    {
        fprintf(stderr, "ssh_channel_open_session() failed.\n");
        return 1;
    }

    char output[1024];
    int bytes_received;
    while ((bytes_received =
                ssh_channel_read(channel, output, sizeof(output), 0)))
    {
        if (bytes_received < 0)
        {
            fprintf(stderr, "ssh_channel_read() failed.\n");
            return 1;
        }
        printf("%.*s", bytes_received, output);
    }
    printf("\n");

    /*
        ssh_channel_send_eof(channel);
        ssh_channel_close(channel);
        ssh_channel_free(channel);
    */
    return 0;
}

int authenticate_pubkey(ssh_session session)
{
    int rc;
    rc = ssh_userauth_publickey_auto(session, NULL, NULL);
    fprintf(stderr, "got rc = %d\n", rc);
    if (rc == SSH_AUTH_ERROR)
    {
        fprintf(stderr, "Authentication failed: %s\n", ssh_get_error(session));
        return SSH_AUTH_ERROR;
    }
    return rc;
}

int verify_knownhost(ssh_session session)
{
    enum ssh_known_hosts_e state;
    unsigned char *hash = NULL;
    ssh_key srv_pubkey = NULL;
    size_t hlen;
    char buf[10];
    char *hexa;
    char *p;
    int cmp;
    int rc;

    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if (rc < 0)
    {
        fprintf(stderr, "ssh_get_server_publickey returns an error\n");
        return -1;
    }

    rc =
        ssh_get_publickey_hash(srv_pubkey, SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
    ssh_key_free(srv_pubkey);
    if (rc < 0)
    {
        fprintf(stderr, "ssh_get_publickey_hash returns an error\n");
        return -1;
    }

    state = ssh_session_is_known_server(session);
    switch (state)
    {
    case SSH_KNOWN_HOSTS_OK:
        /* OK */

        break;
    case SSH_KNOWN_HOSTS_CHANGED:
        fprintf(stderr, "Host key for server changed: it is now:\n");
        ssh_print_hash(SSH_PUBLICKEY_HASH_SHA1, hash, hlen);
        fprintf(stderr, "For security reasons, connection will be stopped\n");
        ssh_clean_pubkey_hash(&hash);

        return -1;
    case SSH_KNOWN_HOSTS_OTHER:
        fprintf(stderr,
                "The host key for this server was not found but an other"
                "type of key exists.\n");
        fprintf(stderr,
                "An attacker might change the default server key to"
                "confuse your client into thinking the key does not exist\n");
        ssh_clean_pubkey_hash(&hash);

        return -1;
    case SSH_KNOWN_HOSTS_NOT_FOUND:
        fprintf(stderr, "Could not find known host file.\n");
        fprintf(stderr,
                "If you accept the host key here, the file will be automatically "
                "created.\n");
        break;

        /* FALL THROUGH to SSH_SERVER_NOT_KNOWN behavior */

    case SSH_KNOWN_HOSTS_UNKNOWN:
        hexa = ssh_get_hexa(hash, hlen);
        fprintf(stderr,
                "The server is unknown. Do you trust the host key (yes/no)?\n");
        fprintf(stderr, "Public key hash: %s\n", hexa);
        ssh_string_free_char(hexa);
        ssh_clean_pubkey_hash(&hash);
        p = fgets(buf, sizeof(buf), stdin);
        if (p == NULL)
        {
            return -1;
        }

        cmp = strncasecmp(buf, "yes", 3);
        if (cmp != 0)
        {
            return -1;
        }

        rc = ssh_session_update_known_hosts(session);
        if (rc < 0)
        {
            fprintf(stderr, "Error %s\n", strerror(errno));
            return -1;
        }

        break;
    case SSH_KNOWN_HOSTS_ERROR:
        fprintf(stderr, "Error %s", ssh_get_error(session));
        ssh_clean_pubkey_hash(&hash);
        return -1;
    }

    ssh_clean_pubkey_hash(&hash);
    return 0;
}

void printerrorcode(int errcode)
{
    fprintf(stderr, "Error in strtoul, got errorcode %d: %s\n", errcode,
            strerror(errcode));
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    ssh_session my_ssh_session;
    ssh_channel my_ssh_channel;
    int verbosity = SSH_LOG_PACKET;
    size_t port;
    char *tmp;
    char *hostname = argv[1];

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s hostname port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // get port number
    errno = 0;
    port = strtoul(argv[2], &tmp, 10);
    if (errno != 0)
        printerrorcode(errno);

    if (port < 1 || port > 65535)
    {
        fprintf(stderr, "Error port out of range\n");
        exit(EXIT_FAILURE);
    }

    errno = 0;
    my_ssh_session = ssh_new();
    if (errno != 0)
        printerrorcode(errno);

    ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, hostname);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);

    int rc = ssh_connect(my_ssh_session);
    if (rc != SSH_OK)
    {
        fprintf(stderr, "Error connecting to %s: %s\n", hostname,
                ssh_get_error(my_ssh_session));
        exit(EXIT_FAILURE);
    }

    rc = verify_knownhost(my_ssh_session);

    rc = authenticate_pubkey(my_ssh_session);
    fprintf(stderr, "authenticate pubkey return %d\n", rc);

    rc = establish_shell_session(my_ssh_session, &my_ssh_channel);
    fprintf(stderr, "Try to establish session, got rc=%d\n", rc);
    if (rc == SSH_OK)
    {
        rc = shell_cmd(my_ssh_channel, "ls -l");
    }

    ssh_channel_close(my_ssh_channel);
    ssh_channel_send_eof(my_ssh_channel);
    ssh_channel_free(my_ssh_channel);
    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
    exit(EXIT_SUCCESS);

    return 0;
}