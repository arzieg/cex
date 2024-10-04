// Programm call
// https://github.com/containers/bubblewrap

#include "utils.h"
#include <argp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // EXIT_FAILURE, EXIT_SUCCESS

/* -------------------------------------------
   Arg Parse
  -------------------------------------------*/

const char *argp_program_version = "filedir 0.1";
const char *argp_program_bug_address = "<bug@to.me>";

/* Program documentation. */
static char doc[]
    = "filedir - a programm to show files in one or more directories.";

/* A description of the arguments we accept. */
static char args_doc[] = "DIRECTORY";

/* The options we understand. */
static struct argp_option options[]
    = { { "verbose", 'v', 0, 0, "Produce verbose output", 0 },
        { "recursive", 'r', 0, 0, "run recursive", 0 },
        { "exclude", 'e', "EXCLUDEFILE", 0, "exclude file <file>", 0 },
        { 0 } };

/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *args[1]; /* DIRECTORY */
  char *exclude;
  bool recursive, verbose;
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
    case 'e':
      arguments->exclude = arg;
      break;
#
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

  /* Default values. */
  arguments.recursive = false;
  arguments.verbose = false;
  arguments.exclude = NULL;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  fprintf (stdout, "ARG0 = %s\n", arguments.args[0]);
  fprintf (stdout, "recursive = %d\n", arguments.recursive);
  fprintf (stdout, "verbose = %d\n", arguments.verbose);
  fprintf (stdout, "EXCLUDE = %s\n", arguments.exclude);

  // if (arguments.pinteractive) interactive();
  // Stack_t *ConfigFiles;
  // if (arguments.confdir) ConfigFiles =
  // get_files_in_confdir(arguments.confdir); if (arguments.confdir)
  // testmain();
  // if (arguments.confdir)
  //  readconf_su (arguments.confdir);

  exit (0);
}
