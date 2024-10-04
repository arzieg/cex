
#if 0
#define debug(...) filedir_log (LOG_DEBUG, __VA_ARGS__)
#else
#define debug(...)
#endif

#define FALSE 0

void filedir_log (int severity, const char *format, ...)
    __attribute__ ((format (printf, 2, 3)));
#define warn(...) filedir_log (LOG_WARNING, __VA_ARGS__)

void die_with_error (const char *format, ...) __attribute__ ((__noreturn__))
__attribute__ ((format (printf, 1, 2)));

void die (const char *format, ...) __attribute__ ((__noreturn__))
__attribute__ ((format (printf, 1, 2)));