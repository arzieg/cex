#include <argp.h>
#include <errno.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "pf.h"

/* -------------------------------------------
   Arg Parse
  -------------------------------------------*/

const char *argp_program_version = "pf 0.1";
const char *argp_program_bug_address = "<bug@to.me>";

/* Program documentation. */
static char doc[] = "pf -- ein Programm zur Pflege der pf-Datei";

/* A description of the arguments we accept. */
static char args_doc[] = "PFFILE";

/* The options we understand. */
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"interactive", 'i', 0, 0, "Interactive Mode"},
    {"output", 'o', "OUTFILE", 0, "Save File to <Outfile>"},
    {"check", 'c', "SID", 0, "Check Configuration of SID"},
    {0}};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[1]; /* PFILE */
  char *output_file;
  char *sid;
  bool pinteractive, verbose;
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key) {
    case 'i':
      arguments->pinteractive = true;
      break;
    case 'v':
      arguments->verbose = true;
      break;
    case 'o':
      arguments->output_file = arg;
      break;
    case 'c':
      arguments->sid = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) /* Too many arguments. */
        argp_usage(state);

      arguments->args[state->arg_num] = arg;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 1) /* Not enough arguments. */
        argp_usage(state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc};

/* ==============
   main
   ==============*/
int main(int argc, char **argv) {
  struct arguments arguments;
  FILE *outstream;

  /* Default values. */
  arguments.sid = "";
  arguments.pinteractive = false;
  arguments.verbose = false;
  arguments.output_file = NULL;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  /* Where do we send output? */
  if (arguments.output_file)
    outstream = fopen(arguments.output_file, "w");
  else
    outstream = stdout;

  fprintf(outstream, "PFFILE = %s\n", arguments.args[0]);
  fprintf(outstream, "SID = %s\n", arguments.sid);
  fprintf(outstream, "interactive = %d\n", arguments.pinteractive);
  fprintf(outstream, "verbose = %d\n", arguments.verbose);
  fprintf(outstream, "OUTPUT_FILE = %s\n", arguments.output_file);

  if (arguments.pinteractive) interactive();
  exit(0);
}