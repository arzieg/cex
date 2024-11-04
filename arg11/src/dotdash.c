#include "dotdash.h"
#include <stdlib.h>
static int
parse_opt (int key, char *arg, struct argp_state *state)
{
  switch (key)
    {
    case 'd':
      {
        unsigned int i;
        unsigned int dots = 1;
        if (arg != NULL)
          dots = atoi (arg);
        for (i = 0; i < dots; i++)
          printf (".");
        break;
      }
    case 888:
      printf ("-");
      break;
    }
  return 0;
}
static struct argp_option options[]
    = { { "dot", 'd', "NUM", OPTION_ARG_OPTIONAL,
          "Show some dots on the screen" },
        { "dash", 888, 0, 0, "Show a dash on the screen" },
        { 0 } };
struct argp dotdash_argp = { options, parse_opt, 0, 0, 0 };