/*
Create a parser to interpret and execute the Deadfish language.

Deadfish operates on a single value in memory, which is initially set to 0.

It uses four single-character commands:

i: Increment the value
d: Decrement the value
s: Square the value
o: Output the value to a result array
All other instructions are no-ops and have no effect.

Examples
Program "iiisdoso" should return numbers [8, 64].
Program "iiisdosodddddiso" should return numbers [8, 64, 3600].

*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int *parse (const char *program);
int *
parse (const char *program)
{
  // return a heap-allocated int array
  // its length shall be at least equal to
  // the count of 'o' commands in the program
  int value = 0;

  int *ret = malloc (sizeof (int) * strlen (program));
  int *r = ret;
  if (ret != NULL)
    {
      for (; *program; program++)
        {
          switch (program[0])
            {
            case 'i':
              value += 1;
              break;
            case 'd':
              value -= 1;
              break;
            case 's':
              value *= value;
              break;
            case 'o':
              *r++ = value;
              break;
            }
        }
    }
  return ret;
}

int
main (int argc, char *argv[])
{

  if (argc != 2)
    {
      printf ("\nerror, call daedfish i|d|s|o");
      return 1;
    }

  int *result = parse (argv[1]);
  int output_count = 0;
  for (const char *p = argv[1]; *p; p++)
    {
      if (*p == 'o')
        {
          output_count++;
        }
    }

  // Print the result array
  printf ("Output: ");
  for (int i = 0; i < output_count; i++)
    {
      printf ("%d ", result[i]);
    }
  printf ("\n");

  // Free the allocated memory
  free (result);

  return 0;
}
