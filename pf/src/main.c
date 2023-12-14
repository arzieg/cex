#include <argp.h>
#include <assert.h>
#include <stdio.h>

/* =============================================== */
/* ARGP */
/* =============================================== */
const char *argp_program_version = "pf 0.1";
const char *argp_program_bug_address = "bug@to.me";

// this struct is used by main to communicate with parse_opt
struct arguments {
  char *args[1];  // arg1
  int verbose;    // -v
  char *outfile;  // arguments for -o
  char *sid;      // arguments for -c
};

// order of field: {NAME, KEY, ARG, FLAGS, DOC}
static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Verbose Output"},
    {"output", 'o', "OUTFILE", 0, "Save File to <Outfile>"},
    {"check", 'c', "SID", 0, "Check Configuration of SID"},
    {0}};

// parser: order of parameters: KEY, ARG, STATE
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  switch (key) {
    case 'v':
      arguments->verbose = 1;
      break;
    case 'c':
      arguments->sid = arg;
      break;
    case 'o':
      arguments->outfile = arg;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 1) {
        argp_usage(state);
      }
      arguments->args[state->arg_num] = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) {
        argp_usage(state);
      }
      arguments->args[state->arg_num] = arg;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

static char args_doc[] = "PFFILE";  // non-option command-line arguments
static char doc[] = "pf -- ein Programm zur Pflege der pf-Datei";
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char **argv) {
  struct arguments arguments;
  FILE *outstream;

  // arguments default
  arguments.outfile = NULL;
  arguments.sid = "";
  arguments.verbose = 0;

  // parsing arguments
  argp_parse(&argp, argc, argv, 0, 0, &arguments);

  /* Where do we send output? */
  if (arguments.outfile)
    outstream = fopen(arguments.outfile, "w");
  else
    outstream = stdout;

  /* Print argument values */
  fprintf(outstream, "sid = %s\n\n", arguments.sid);
  fprintf(outstream, "PFFILE = %s\n\n", arguments.args[0]);

  /* If in verbose mode, print song stanza */
  if (arguments.verbose) fprintf(outstream, "Verbose ON");

  return 0;
}