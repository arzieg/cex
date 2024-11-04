#include "dotdash.h"
#include <argp.h>
#include <stdio.h>
static int
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case 999:
      printf ("... --- ...");
      break;
    }
  return 0;
}
int
main (int argc, char **argv)
{
  struct argp_option options[]
      = { { "SOS", 999, 0, 0, "Show the SOS sequence on the screen" }, { 0 } };

  struct argp_child children_parsers[]
      = { { &dotdash_argp, 0, "Basic Morse Code Options:", 7 }, { 0 } };

  struct argp argp = { options, parse_opt, 0, 0, children_parsers };
  int retval = argp_parse (&argp, argc, argv, 0, 0, 0);
  printf ("\n");
  return retval;
}