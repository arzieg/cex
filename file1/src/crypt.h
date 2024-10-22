#ifndef _CRYPT

#include <openssl/types.h>
#include <stddef.h>

void generate_rsa_keys (const char *pub_filename, const char *priv_filename);
size_t save_encrypted_key_iv (const char *filename, unsigned char *key,
                              size_t key_len, unsigned char *iv, size_t iv_len,
                              const char *pubkeyfile);
size_t load_decrypted_key_iv (const char *filename, unsigned char *key,
                              size_t key_len, unsigned char *iv, size_t iv_len,
                              const char *privkeyfile);
void handle_errors (void);

#endif