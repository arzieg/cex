#include "utils.h"
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
die_with_error (const char *format, ...)
{
  va_list args;
  // int errsv;

  // errsv = errno;

  va_start (args, format);
  // bwrap_logv (LOG_ERR, format, args, strerror (errsv));
  va_end (args);

  exit (1);
}

void
die (const char *format, ...)
{
  va_list args;

  va_start (args, format);
  // bwrap_logv (LOG_ERR, format, args, NULL);
  va_end (args);

  exit (1);
}

size_t
check_file_access (char *filename)
{
  if (access (filename, R_OK) != 0)
    {
      return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}