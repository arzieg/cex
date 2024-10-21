// Programm call
// https://github.com/containers/bubblewrap

#include "crypt.h"
#include "filedir.h"
#include "utils.h"
#include <argp.h>
#include <crypt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS
#include <unistd.h>

/*
  --verbose --recursive --exclude --encrypt|decrypt --keyfile
*/

/* -------------------------------------------
   Arg Parse Initialization
  -------------------------------------------*/

const char *argp_program_version = "filedir 0.1";
const char *argp_program_bug_address = "<bug@to.me>";

/* Program purpose. */
static char doc[]
    = "filedir - a programm to show files in one or more directories.";

/* A description of the arguments we accept. */
static char args_doc[] = "DIRECTORY";

/* The options we understand. */
static struct argp_option options[]
    = { { "verbose", 'v', 0, 0, "Produce verbose output", 0 },
        { "recursive", 'r', 0, 0, "run recursive", 0 },
        { "encrypt", 'e', 0, 0, "encrypt files", 0 },
        { "decrypt", 'd', 0, 0, "decrypt files", 0 },
        { "exclude", 'x', "EXCLUDEFILE", 0, "exclude file <file>", 0 },
        { "keyfile", 'k', "KEYFILE", 0, "private keyfile <file>", 0 },
        { 0 } };

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[1]; /* DIRECTORY */
  char *exclude, *keyfile;
  bool recursive, verbose, encrypt, decrypt;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'r':
      arguments->recursive = true;
      break;
    case 'v':
      arguments->verbose = true;
      break;
    case 'x':
      arguments->exclude = arg;
      break;
    case 'k':
      arguments->keyfile = arg;
      break;
    case 'e':
      if (arguments->decrypt)
        {
          argp_error (
              state,
              "Options --encrypt and --decrypt cannot be used together.");
        }
      arguments->encrypt = true;
      break;
    case 'd':
      if (arguments->encrypt)
        {
          argp_error (
              state,
              "Options --encrypt and --decrypt cannot be used together.");
        }
      arguments->decrypt = true;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) /* Too many arguments. */
        argp_usage (state);
      arguments->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) /* Not enough arguments. */
        argp_usage (state);
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

/* ==============
   main
   ==============*/
int
main (int argc, char **argv)
{
  struct arguments arguments;
  unsigned char key[32] = "HalloIchwarInBremenImLetztenSomm";
  unsigned char iv[16] = "123456789abcdeF";

  /* Default values. */
  arguments.recursive = false;
  arguments.verbose = false;
  arguments.exclude = NULL;
  arguments.keyfile = NULL;
  arguments.decrypt = false;
  arguments.encrypt = false;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  fprintf (stdout, "ARG0 = %s\n", arguments.args[0]);
  fprintf (stdout, "recursive = %d\n", arguments.recursive);
  fprintf (stdout, "verbose = %d\n", arguments.verbose);
  fprintf (stdout, "EXCLUDE = %s\n", arguments.exclude);
  fprintf (stdout, "KEYFILE = %s\n", arguments.keyfile);
  fprintf (stdout, "encrypt = %d\n", arguments.encrypt);
  fprintf (stdout, "decrypt = %d\n", arguments.decrypt);

  // exists exclude file?
  if (arguments.exclude)
    {
      if (access (arguments.exclude, R_OK) != 0)
        {
          fprintf (stderr, "Could not access excludefile %s.\n",
                   arguments.exclude);
          exit (EXIT_FAILURE);
        }
    }

  // exists keyfile file?
  if (arguments.keyfile)
    {
      if (access (arguments.keyfile, R_OK) != 0)
        {
          fprintf (stderr, "Could not access private keyfile %s.\n",
                   arguments.keyfile);
          exit (EXIT_FAILURE);
        }
    }

  // save_encrypted_key_iv ("encrypted.bin", key, sizeof (key), iv, sizeof
  // (iv),
  //                        arguments.keyfile);

  load_decrypted_key_iv ("encrypted.bin", key, sizeof (key), iv, sizeof (iv),
                         arguments.keyfile);
  // load_encrypted_key_iv ("encrypted.bin", key, iv, "private.pem");
  printf ("\nkey = %s", key);
  printf ("\niv = %s", iv);
  // get_dir (arguments.args[0], arguments.recursive, arguments.exclude);

  exit (0);
}
