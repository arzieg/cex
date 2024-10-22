#ifndef _CRYPT

#include <openssl/types.h>
#include <stddef.h>

void generate_rsa_keys (const char *pub_filename, const char *priv_filename);

void handle_errors (void);

#endif