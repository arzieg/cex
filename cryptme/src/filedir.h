
#ifndef _FILEDIR

#include <stdbool.h>

void get_dir (char *path, bool recursive, char *exclude, bool encrypt,
              bool decrypt, unsigned char *key, unsigned char *iv);
#endif