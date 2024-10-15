#define _DEFAULT_SOURCE

#include "filedir.h"
#include "utils.h"
#include <dirent.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NORMAL "\x1B[0m"
#define GREEN "\x1B[32m"
#define BLUE "\x1B[34m"
#define RED "\x1B[31m"

void
get_dir (char *path, bool recursive, char *exclude)
{
  DIR *d = opendir (path);
  if (d == NULL)
    return;
  struct dirent *dir;
  while ((dir = readdir (d)) != NULL)
    {
      if (dir->d_type != DT_DIR)
        {
          bool skip = false;
          char d_filename[257];
          snprintf (d_filename, sizeof (d_filename), "%s/%s", path,
                    dir->d_name);
          FILE *file = fopen (exclude, "r");
          if (file == NULL)
            {
              perror ("Error opening file");
              exit (EXIT_FAILURE);
            }

          // Buffer to store each line
          char pattern[256];

          // Read each line from the file
          while (fgets (pattern, sizeof (pattern), file))
            {
              pattern[strcspn (pattern, "\n")] = 0;
              DEBUG_PRINT ("%sSearch for Pattern %s in file %s\n", NORMAL,
                           pattern, d_filename);
              regex_t regex;
              int reti;

              // compile regex
              reti = regcomp (&regex, pattern, REG_EXTENDED);
              if (reti)
                {
                  fprintf (stderr, "Could not compile regex\n");
                  exit (EXIT_FAILURE);
                }
              reti = regexec (&regex, d_filename, 0, NULL, 0);
              if (!reti)
                {
                  skip = true;
                  continue;
                }
              // Free memory allocated to the pattern buffer by regcomp()
              regfree (&regex);
            }

          // Close the file
          fclose (file);
          if (skip)
            printf ("  %s<FILE>%s\n", GREEN, d_filename);
          else
            printf ("  %s<FILE>%s\n", BLUE, d_filename);
        }
      else if (dir->d_type == DT_DIR && strcmp (dir->d_name, ".") != 0
               && strcmp (dir->d_name, "..") != 0)
        {
          // printf ("%s<DIR> %s\n", GREEN, dir->d_name);
          char d_path[257];
          snprintf (d_path, sizeof (d_path), "%s/%s", path, dir->d_name);
          if (recursive)
            get_dir (d_path, recursive, exclude);
        }
    }
  closedir (d);
}
