#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <stdlib.h>

/*
 Generate the private key:
        openssl genpkey -algorithm RSA -out private.pem -aes256
Extract the public key:
        openssl rsa -pubout -in private.pem -out public.pem

*/

void
save_encrypted_key_iv (const char *filename, unsigned char *key,
                       unsigned char *iv, const char *pubkey_filename)
{
  FILE *pubkey_file = fopen (pubkey_filename, "rb");
  EVP_PKEY *pubkey = PEM_read_PUBKEY (pubkey_file, NULL, NULL, NULL);
  fclose (pubkey_file);

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new (pubkey, NULL);
  EVP_PKEY_encrypt_init (ctx);

  unsigned char encrypted_key[256];
  unsigned char encrypted_iv[256];
  size_t encrypted_key_len, encrypted_iv_len;

  EVP_PKEY_encrypt (ctx, NULL, &encrypted_key_len, key, 32);
  EVP_PKEY_encrypt (ctx, NULL, &encrypted_iv_len, iv, 16);
  EVP_PKEY_encrypt (ctx, encrypted_key, &encrypted_key_len, key, 32);
  EVP_PKEY_encrypt (ctx, encrypted_iv, &encrypted_iv_len, iv, 16);

  FILE *file = fopen (filename, "wb");
  fwrite (encrypted_key, 1, encrypted_key_len, file);
  fwrite (encrypted_iv, 1, encrypted_iv_len, file);
  fclose (file);

  EVP_PKEY_CTX_free (ctx);
  EVP_PKEY_free (pubkey);
}

void
load_encrypted_key_iv (const char *filename, unsigned char *key,
                       unsigned char *iv, const char *privkey_filename)
{
  FILE *privkey_file = fopen (privkey_filename, "rb");
  EVP_PKEY *privkey = PEM_read_PrivateKey (privkey_file, NULL, NULL, NULL);
  fclose (privkey_file);

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new (privkey, NULL);
  EVP_PKEY_decrypt_init (ctx);

  unsigned char encrypted_key[256];
  unsigned char encrypted_iv[256];
  size_t encrypted_key_len = 256, encrypted_iv_len = 256;

  FILE *file = fopen (filename, "rb");
  fread (encrypted_key, 1, encrypted_key_len, file);
  fread (encrypted_iv, 1, encrypted_iv_len, file);
  fclose (file);

  EVP_PKEY_decrypt (ctx, key, &encrypted_key_len, encrypted_key,
                    encrypted_key_len);
  EVP_PKEY_decrypt (ctx, iv, &encrypted_iv_len, encrypted_iv,
                    encrypted_iv_len);

  EVP_PKEY_CTX_free (ctx);
  EVP_PKEY_free (privkey);
}

int
main ()
{
  unsigned char key[32]; // 256-bit key
  unsigned char iv[16];  // 128-bit IV

  // Generate AES key and IV
  if (!RAND_bytes (key, sizeof (key)) || !RAND_bytes (iv, sizeof (iv)))
    {
      fprintf (stderr, "Error generating random bytes.\n");
      return 1;
    }

  // Save encrypted key and IV
  save_encrypted_key_iv ("encrypted_key_iv.bin", key, iv, "public.pem");

  // Load encrypted key and IV
  unsigned char decrypted_key[32];
  unsigned char decrypted_iv[16];
  load_encrypted_key_iv ("encrypted_key_iv.bin", decrypted_key, decrypted_iv,
                         "private.pem");

  return 0;
}
