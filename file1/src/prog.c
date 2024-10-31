// Programm call
// https://github.com/containers/bubblewrap

#include "crypt.h"
#include "filedir.h"
#include "utils.h"
#include <argp.h>
#include <stdbool.h> // true, false
#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS
#include <string.h> // strlen

/* -------------------------------------------
   Arg Parse Initialization
  -------------------------------------------*/

/* TODO

  createkey speichert aktuell in eine encryped.bin. hier soll per parameter die
  encrypted.bin angegeben werden können. diese kann ja auch woanders stehen als
  im Verzeichnis von filedir. save_encrypted_key_iv ("encrypted.bin", key,
  sizeof (key), iv, sizeof (iv), arguments.pubkey);


*/

const char *argp_program_version = "cryptme 0.1";
const char *argp_program_bug_address = "<bug@to.me>";

/* Program purpose. */
static char doc[] = "cryptme - a programm to encrypt / decrypt files in one "
                    "or more sub directories.";

/* A description of the arguments we accept. */
static char args_doc[] = "DIRECTORY";

/* The options we understand. */
static struct argp_option options[]
    = { { 0, 0, 0, 0, "encryption/decryption options:", 3 },
        { "verbose", 'v', 0, 0, "Produce verbose output", 0 },
        { "recursive", 'r', 0, 0, "run recursive", 0 },
        { "encrypt", 'e', 0, 0, "encrypt files", 0 },
        { "decrypt", 'd', 0, 0, "decrypt files", 0 },
        { "exclude", 'x', "EXCLUDEFILE", 0, "exclude file <file>", 0 },
        { 0, 0, 0, 0, "Options in createkeys and encryption/decryption:", 5 },
        { "privkey", 'k', "PRIV-KEY", 0, "private keyfile <file>", 0 },
        { "pubkey", 'p', "PUB-KEY", 0, "public keyfile <file>", 0 },
        { 0, 0, 0, 0, "Createkey options:", 7 },
        { "createkeys", 'c', 0, 0, "create key files", 0 },
        { "aeskey", 'a', "AES-KEY", 0,
          "key to use for encryption with aes (32 characters)", 0 },
        { "ivkey", 'i', "IV-KEY", 0,
          "initialization vector to use encryption for aes (16 characters)",
          0 },
        { 0 } };

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[1]; /* DIRECTORY */
  char *exclude, *privkey, *pubkey, *aeskey, *ivkey;
  bool recursive, verbose, encrypt, decrypt, createkeys;
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
    case 'c':
      arguments->createkeys = true;
      break;
    case 'k':
      arguments->privkey = arg;
      break;
    case 'p':
      arguments->pubkey = arg;
      break;
    case 'a':
      arguments->aeskey = arg;
      break;
    case 'i':
      arguments->ivkey = arg;
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
    } // end of switch

  // TODO
  // check if arguments.pubkey and arguments.privkey are both set. If not then
  // arpg_error (state, "Both parameter have to be set!")
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
  // unsigned char key[32] = "HalloIchwarInBremenImLetztenSomm";
  // unsigned char iv[16] = "123456789abcdeF";
  unsigned char key[32];
  unsigned char iv[16];

  /* Default values. */
  arguments.recursive = false;
  arguments.verbose = false;
  arguments.exclude = NULL;
  arguments.privkey = NULL;
  arguments.pubkey = NULL;
  arguments.aeskey = NULL;
  arguments.ivkey = NULL;
  arguments.createkeys = false;
  arguments.decrypt = false;
  arguments.encrypt = false;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  fprintf (stdout, "ARG0 = %s\n", arguments.args[0]);
  fprintf (stdout, "recursive = %d\n", arguments.recursive);
  fprintf (stdout, "verbose = %d\n", arguments.verbose);
  fprintf (stdout, "EXCLUDE = %s\n", arguments.exclude);
  fprintf (stdout, "CREATEKEYS = %d\n", arguments.createkeys);
  fprintf (stdout, "PRIVKEY = %s\n", arguments.privkey);
  fprintf (stdout, "PUBKEY = %s\n", arguments.pubkey);
  fprintf (stdout, "AESKEY = %s\n", arguments.aeskey);
  fprintf (stdout, "IVKEY = %s\n", arguments.ivkey);
  fprintf (stdout, "encrypt = %d\n", arguments.encrypt);
  fprintf (stdout, "decrypt = %d\n", arguments.decrypt);

  /*
   Parameter check
  */
  if (arguments.exclude)
    {
      if (check_file_access (arguments.exclude) == EXIT_FAILURE)
        {
          fprintf (stderr, "Could not access file %s.\n", arguments.exclude);
          exit (EXIT_FAILURE);
        }
    }

  // aeskey 32 bytes?
  if (arguments.aeskey)
    {
      if (strlen (arguments.aeskey) != 32)
        {
          fprintf (stderr,
                   "\nAES Key has to be exact 32 Characters long, found %ld "
                   "characters\n",
                   strlen (arguments.aeskey));
          exit (EXIT_FAILURE);
        }
    }

  // initializue vector iv = 16 bytes
  if (arguments.ivkey)
    {
      if (strlen (arguments.ivkey) != 16)
        {
          fprintf (
              stderr,
              "\nInitialization vector IV-KEY has to be exact 16 Characters "
              "long, found %ld "
              "characters\n",
              strlen (arguments.ivkey));
          exit (EXIT_FAILURE);
        }
    }

  /*
     ---------------------------
     logic check
     ---------------------------
  */

  // if createkey is set, then encrypt or decrypt must be unset.
  if (arguments.createkeys && (arguments.encrypt || arguments.decrypt))
    {
      printf (
          "\nRun program with --createkey or --encrypt/--decrypt but not with "
          "both\n");
      exit (EXIT_FAILURE);
    }

  // if encrypt or decrypt, then priv.key and pub.key must be set
  if (arguments.encrypt || arguments.decrypt)
    {
      if (!arguments.privkey || !arguments.pubkey)
        {
          printf ("\nPlease set private and public key!\n");
          exit (EXIT_FAILURE);
        }
      if (arguments.privkey)
        {
          if (check_file_access (arguments.privkey) == EXIT_FAILURE)
            {
              fprintf (stderr, "Could not access file %s.\n",
                       arguments.privkey);
              exit (EXIT_FAILURE);
            }
        }
      if (arguments.pubkey)
        {
          if (check_file_access (arguments.pubkey) == EXIT_FAILURE)
            {
              fprintf (stderr, "Could not access file %s.\n",
                       arguments.pubkey);
              exit (EXIT_FAILURE);
            }
        }
    }

  // if createkey is set, then priv.key, pub.key, iv and aes must be set. If
  // priv.key or pub.key exist, then do not create new keys and exit with
  // message
  if (arguments.createkeys)
    {
      if (!arguments.privkey || !arguments.pubkey || !arguments.ivkey
          || !arguments.aeskey)
        {
          printf ("\nPlease provide private and public keyfile, iv and aes "
                  "string\n");
          exit (EXIT_FAILURE);
        }
      if (arguments.privkey)
        {
          if (check_file_access (arguments.privkey) == EXIT_SUCCESS)
            {
              printf (
                  "\nFound private keyfile %s. I do not overwrite the file, "
                  "please remove manually\n",
                  arguments.privkey);
              exit (EXIT_FAILURE);
            }
        }
      if (arguments.pubkey)
        {

          if (check_file_access (arguments.pubkey) == EXIT_SUCCESS)
            {
              printf (
                  "\nFound public keyfile %s. I do not overwrite the file, "
                  "please remove manually\n",
                  arguments.pubkey);
              exit (EXIT_FAILURE);
            }
        }

      generate_rsa_keys (arguments.pubkey, arguments.privkey);
      // TODO: make encrypted.bin variable
      save_encrypted_key_iv ("encrypted.bin", key, sizeof (key), iv,
                             sizeof (iv), arguments.pubkey);
    }

  exit (EXIT_SUCCESS); // Programm Stop

  /*
  das noch nicht weiter verarbeitet
  */

  if (arguments.encrypt || arguments.decrypt)
    {
      load_decrypted_key_iv ("encrypted.bin", key, sizeof (key), iv,
                             sizeof (iv), arguments.privkey);
    }

  get_dir (arguments.args[0], arguments.recursive, arguments.exclude,
           arguments.encrypt, arguments.decrypt, key, iv);

  // load_encrypted_key_iv ("encrypted.bin", key, iv, "private.pem");
  printf ("\nkey = %s", key);
  printf ("\niv = %s", iv);
  // get_dir (arguments.args[0], arguments.recursive, arguments.exclude);

  exit (0);
}
