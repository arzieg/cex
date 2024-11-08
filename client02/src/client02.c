#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  TODO

  ssh_channel_write ended mit einem rc=2 ssh_error.
  unklar warum, da channel und session eröffnet werden.

*/

int
open_remote_shell (ssh_session session)
{
  ssh_channel channel;
  int rc;
  char buffer[256];
  int nbytes;

  // Open a new channel
  channel = ssh_channel_new (session);
  if (channel == NULL)
    {
      fprintf (stderr, "Error: Could not create channel\n");
      return SSH_ERROR;
    }

  rc = ssh_channel_open_session (channel);
  if (rc != SSH_OK)
    {
      fprintf (stderr, "Error: Could not open channel: %s\n",
               ssh_get_error (session));
      ssh_channel_free (channel);
      return SSH_ERROR;
    }
  // Request a shell
  rc = ssh_channel_request_shell (channel);
  if (rc != SSH_OK)
    {
      fprintf (stderr, "Error: Could not request shell: %s\n",
               ssh_get_error (session));
      ssh_channel_close (channel);
      ssh_channel_free (channel);
      return SSH_ERROR;
    }

  // Interact with the shell
  while (ssh_channel_is_open (channel) && !ssh_channel_is_eof (channel))
    {
      printf ("Remote command to execute: ");
      char command[128];

      char *rv = fgets (command, sizeof (command), stdin);
      if (!rv)
        {
          fprintf (stderr, "Error with fgets");
          return 1;
        }

      command[strlen (command) - 1] = 0;

      int rc = ssh_channel_write (channel, command, strlen (command));
      if (rc != SSH_OK)
        {
          fprintf (stderr, "ssh_channel_write() failed.\n");
          ssh_channel_close (channel);
          ssh_channel_free (channel);
          return rc;
        }

      rc = ssh_channel_write (channel, "\n", 1);
      if (rc != SSH_OK)
        {
          fprintf (stderr, "Error: Could not write newline to channel: %s\n",
                   ssh_get_error (session));
          ssh_channel_close (channel);
          ssh_channel_free (channel);
          return SSH_ERROR;
        }

      nbytes = ssh_channel_read (channel, buffer, sizeof (buffer), 0);
      if (nbytes > 0)
        {
          fwrite (buffer, 1, nbytes, stdout);
          fflush (stdout);
        }
      else if (nbytes < 0)
        {
          fprintf (stderr, "Error: Could not read from channel\n");
          break;
        }

      // Here you can write to the channel if needed
      // Example: ssh_channel_write(channel, "your_command\n",
      // strlen("your_command\n"));
    }

  // Close and free the channel
  ssh_channel_send_eof (channel);
  ssh_channel_close (channel);
  ssh_channel_free (channel);

  return SSH_OK;
}

int
main (int argc, char *argv[])
{
  const char *hostname = 0;
  int port = 22;
  if (argc < 2)
    {
      fprintf (stderr, "Usage: ssh_connect hostname port\n");
      return 1;
    }
  hostname = argv[1];
  if (argc > 2)
    port = atol (argv[2]);

  ssh_session session = ssh_new ();
  if (!session)
    {
      fprintf (stderr, "ssh_new() failed.\n");
      return 1;
    }
  // Options
  ssh_options_set (session, SSH_OPTIONS_HOST, hostname);
  ssh_options_set (session, SSH_OPTIONS_PORT, &port);
  int verbosity = SSH_LOG_PROTOCOL;
  ssh_options_set (session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);

  int ret = ssh_connect (session);
  if (ret != SSH_OK)
    {
      fprintf (stderr, "ssh_connect() failed.\n%s\n", ssh_get_error (session));
      return 1;
    }
  printf ("Connected to %s on port %d.\n", hostname, port);
  printf ("Banner:\n%s\n", ssh_get_serverbanner (session));

  ssh_key key;
  if (ssh_get_server_publickey (session, &key) != SSH_OK)
    {
      fprintf (stderr, "ssh_get_server_publickey() failed.\n%s\n",
               ssh_get_error (session));
      return -1;
    }

  unsigned char *hash;
  size_t hash_len;
  if (ssh_get_publickey_hash (key, SSH_PUBLICKEY_HASH_SHA256, &hash, &hash_len)
      != SSH_OK)
    {
      fprintf (stderr, "ssh_get_publickey_hash() failed.\n%s\n",
               ssh_get_error (session));
      return -1;
    }

  printf ("Host public key hash:\n");
  ssh_print_hash (SSH_PUBLICKEY_HASH_SHA256, hash, hash_len);

  ssh_clean_pubkey_hash (&hash);
  ssh_key_free (key);

  printf ("Checking ssh_session_is_known_server()\n");
  enum ssh_known_hosts_e known = ssh_session_is_known_server (session);
  switch (known)
    {
    case SSH_KNOWN_HOSTS_OK:
      printf ("Host Known.\n");
      break;

    case SSH_KNOWN_HOSTS_CHANGED:
      printf ("Host Changed.\n");
      break;
    case SSH_KNOWN_HOSTS_OTHER:
      printf ("Host Other.\n");
      break;
    case SSH_KNOWN_HOSTS_UNKNOWN:
      printf ("Host Unknown.\n");
      break;
    case SSH_KNOWN_HOSTS_NOT_FOUND:
      printf ("No host file.\n");
      break;

    case SSH_KNOWN_HOSTS_ERROR:
      printf ("Host error. %s\n", ssh_get_error (session));
      return 1;

    default:
      printf ("Error. Known: %d\n", known);
      return 1;
    }

  if (known == SSH_KNOWN_HOSTS_CHANGED || known == SSH_KNOWN_HOSTS_OTHER
      || known == SSH_KNOWN_HOSTS_UNKNOWN
      || known == SSH_KNOWN_HOSTS_NOT_FOUND)
    {
      printf ("Do you want to accept and remember this host? Y/N\n");
      char answer[10];
      char *rv = fgets (answer, sizeof (answer), stdin);
      if (!rv)
        {
          fprintf (stderr, "Error calling fgets\n");
          return 1;
        }
      if (answer[0] != 'Y' && answer[0] != 'y')
        {
          return 0;
        }

      ssh_session_update_known_hosts (session);
    }

  printf ("Password: ");
  char password[128];
  char *rv = fgets (password, sizeof (password), stdin);
  if (!rv)
    {
      fprintf (stderr, "Error calling fgets\n");
      return 1;
    }
  password[strlen (password) - 1] = 0;

  if (ssh_userauth_password (session, 0, password) != SSH_AUTH_SUCCESS)
    {
      fprintf (stderr, "ssh_userauth_password() failed.\n%s\n",
               ssh_get_error (session));
      return 0;
    }
  else
    {
      printf ("Authentication successful!\n");
    }

  if (open_remote_shell (session) != SSH_OK)
    {
      fprintf (stderr, "Error: Could not open remote shell\n");
    }

  ssh_disconnect (session);
  ssh_free (session);

  return 0;
}