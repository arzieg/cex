#include "crypt.h"
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LENGTH 2048
#define PUB_EXP 3
#define PRINT_KEYS

/*
 Generate the private key:
        openssl genpkey -algorithm RSA -out private.pem -aes256
Extract the public key:
        openssl rsa -pubout -in private.pem -out public.pem
Encrypt file:
        openssl rsautl -encrypt -pubin -inkey public.pem -in plaintext.txt -out
encrypted.bin

Decrypt file
         openssl rsautl -decrypt -inkey private.pem -in encrypted.bin -out
decrypted.txt

*/

void
handle_errors (void)
{
  FILE *error_log = fopen ("error.log", "a");
  if (error_log != NULL)
    {
      ERR_print_errors_fp (error_log);
      fclose (error_log);
    }
  else
    {
      // Handle the error of not being able to open the log file
      perror ("Failed to open error.log");
    }
  abort ();
}

void
generate_rsa_keys (const char *pub_filename, const char *priv_filename)
{
  EVP_PKEY *pkey = EVP_RSA_gen (KEY_LENGTH);
  if (!pkey)
    handle_errors ();

  // Save public key
  FILE *pub_file = fopen (pub_filename, "wb");
  if (!pub_file)
    {
      perror ("Unable to open file for writing public key");
      exit (EXIT_FAILURE);
    }
  PEM_write_PUBKEY (pub_file, pkey);
  fclose (pub_file);

  // Save private key
  FILE *priv_file = fopen (priv_filename, "wb");
  if (!priv_file)
    {
      perror ("Unable to open file for writing private key");
      exit (EXIT_FAILURE);
    }
  PEM_write_PrivateKey (priv_file, pkey, NULL, NULL, 0, NULL, NULL);
  fclose (priv_file);

  EVP_PKEY_free (pkey);
}

size_t
save_encrypted_key_iv (const char *filename, unsigned char *key,
                       size_t key_len, unsigned char *iv, size_t iv_len,
                       const char *pubkeyfile)
{
  // Load RSA public key
  FILE *pubkey_file = fopen (pubkeyfile, "r");
  if (!pubkey_file)
    {
      perror ("Unable to open public key file");
      return 1;
    }
  EVP_PKEY *pubkey = PEM_read_PUBKEY (pubkey_file, NULL, NULL, NULL);
  fclose (pubkey_file);

  if (!pubkey)
    {
      fprintf (stderr, "Error reading public key.\n");
      return EXIT_FAILURE;
    }

  FILE *file = fopen (filename, "wb");
  if (!file)
    {
      perror ("Failed to open file for writing");
      exit (EXIT_FAILURE);
    }

  unsigned char *plaintext = malloc (key_len + iv_len);
  if (!plaintext)
    {
      perror ("Failed to allocate memory");
      exit (EXIT_FAILURE);
    }
  memcpy (plaintext, key, key_len);
  memcpy (plaintext + key_len, iv, iv_len);

  unsigned char encrypted[256];
  size_t encrypted_len;
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new (pubkey, NULL);
  if (!ctx)
    handle_errors ();

  if (EVP_PKEY_encrypt_init (ctx) <= 0)
    handle_errors ();
  if (EVP_PKEY_encrypt (ctx, NULL, &encrypted_len, plaintext, key_len + iv_len)
      <= 0)
    handle_errors ();

  if (EVP_PKEY_encrypt (ctx, encrypted, &encrypted_len, plaintext,
                        key_len + iv_len)
      <= 0)
    handle_errors ();

  fwrite (encrypted, 1, encrypted_len, file);
  fclose (file);
  EVP_PKEY_free (pubkey);
  EVP_PKEY_CTX_free (ctx);
  free (plaintext);
  return EXIT_SUCCESS;
}

size_t
load_decrypted_key_iv (const char *filename, unsigned char *key,
                       size_t key_len, unsigned char *iv, size_t iv_len,
                       const char *privkeyfile)
{
  FILE *privkey_file = fopen (privkeyfile, "r");
  if (!privkey_file)
    {
      perror ("Unable to open private key file");
      return EXIT_FAILURE;
    }
  EVP_PKEY *privkey = PEM_read_PrivateKey (privkey_file, NULL, NULL, NULL);
  fclose (privkey_file);

  if (!privkey)
    {
      fprintf (stderr, "Error reading private key.\n");
      return EXIT_FAILURE;
    }
  FILE *file = fopen (filename, "rb");
  if (!file)
    {
      perror ("Failed to open file for reading");
      exit (EXIT_FAILURE);
    }

  unsigned char encrypted[256];
  fread (encrypted, 1, sizeof (encrypted), file);
  fclose (file);

  unsigned char *plaintext = malloc (key_len + iv_len);
  size_t plaintext_len;

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new (privkey, NULL);
  if (!ctx)
    handle_errors ();

  if (EVP_PKEY_decrypt_init (ctx) <= 0)
    handle_errors ();
  if (EVP_PKEY_decrypt (ctx, NULL, &plaintext_len, encrypted,
                        sizeof (encrypted))
      <= 0)
    handle_errors ();
  if (EVP_PKEY_decrypt (ctx, plaintext, &plaintext_len, encrypted,
                        sizeof (encrypted))
      <= 0)
    handle_errors ();

  memcpy (key, plaintext, key_len);
  memcpy (iv, plaintext + key_len, iv_len);

  EVP_PKEY_free (privkey);
  EVP_PKEY_CTX_free (ctx);
  free (plaintext);
  return EXIT_SUCCESS;
}
