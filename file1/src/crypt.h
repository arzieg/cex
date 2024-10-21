#ifndef _CRYPT

#include <openssl/types.h>
#include <stddef.h>

size_t save_encrypted_key_iv (const char *filename, unsigned char *key,
                              size_t key_len, unsigned char *iv, size_t iv_len,
                              const char *pubkey_file);
size_t load_decrypted_key_iv (const char *filename, unsigned char *key,
                              size_t key_len, unsigned char *iv, size_t iv_len,
                              const char *privatekey_file);
void handle_errors (void);

EVP_PKEY *load_public_key (const char *filename);
EVP_PKEY *load_private_key (const char *filename);

#endif