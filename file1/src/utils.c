#include "utils.h"
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

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
