#define _DEFAULT_SOURCE

#include "filedir.h"
#include "crypt.h"
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
get_dir (char *path, bool recursive, char *exclude, bool encrypt, bool decrypt,
         unsigned char *key, unsigned char *iv)
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
          if (exclude) // handle exlude file
            {
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
            }
          if (skip)
            printf ("%s<FILE>%s\n", GREEN, d_filename);
          else
            {
              printf ("%s<FILE>%s", BLUE, d_filename);
              // crypt?
              if (encrypt)
                {
                  char d_filenameout[263];
                  strcpy (d_filenameout, d_filename);
                  strcat (d_filenameout, ".crypt");
                  do_crypt (d_filename, d_filenameout, 1, key, iv);
                  printf ("%s... encrypt file to ... %s%s\n", RED,
                          d_filenameout, NORMAL);
                  remove (d_filename);
                }
              else
                printf ("\n");

              /* check also for the ending .crypt */
              if (decrypt)
                {
                  char d_filenameout[263];
                  const char *search = ".crypt";
                  size_t str_len = strlen (d_filename);
                  size_t search_len = strlen (search);

                  if (str_len >= search_len
                      && strcmp (d_filename + str_len - search_len, search)
                             == 0)
                    {
                      strncpy (d_filenameout, d_filename,
                               str_len - search_len);
                      d_filenameout[str_len - search_len] = '\0';
                      do_crypt (d_filename, d_filenameout, 0, key, iv);
                      printf ("%s... decrypt file to ... %s%s\n", RED,
                              d_filenameout, NORMAL);
                      // remove (d_filename);
                    }
                }
              else
                printf ("\n");
            }
        }
      else if (dir->d_type == DT_DIR && strcmp (dir->d_name, ".") != 0
               && strcmp (dir->d_name, "..") != 0)
        {
          // printf ("%s<DIR> %s\n", GREEN, dir->d_name);
          char d_path[257];
          snprintf (d_path, sizeof (d_path), "%s/%s", path, dir->d_name);
          if (recursive)
            get_dir (d_path, recursive, exclude, encrypt, decrypt, key, iv);
        }
    }
  closedir (d);
}
