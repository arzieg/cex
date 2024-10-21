#include "crypt.h"
#include <openssl/err.h>
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
  ERR_print_errors_fp (stderr);
  abort ();
}

EVP_PKEY *
load_public_key (const char *filename)
{
  FILE *pubkey_file = fopen (filename, "r");
  if (!pubkey_file)
    {
      perror ("Unable to open public key file");
      return NULL;
    }

  EVP_PKEY *pubkey = PEM_read_PUBKEY (pubkey_file, NULL, NULL, NULL);
  fclose (pubkey_file);

  if (!pubkey)
    {
      handle_errors ();
    }

  return pubkey;
}

EVP_PKEY *
load_private_key (const char *filename)
{
  FILE *privatekey_file = fopen (filename, "r");
  if (!privatekey_file)
    {
      perror ("Unable to open private key file");
      return NULL;
    }
  EVP_PKEY *privatekey
      = PEM_read_PrivateKey (privatekey_file, NULL, NULL, NULL);
  fclose (privatekey_file);

  if (!privatekey)
    {
      handle_errors ();
    }

  return privatekey;
}

/*
        Save key and iv in a encrypted file
*/
size_t
save_encrypted_key_iv (const char *filename, unsigned char *key,
                       size_t key_len, unsigned char *iv, size_t iv_len,
                       const char *pubkey_file)
{
  EVP_PKEY *pubkey = load_public_key (pubkey_file);
  if (!pubkey)
    {
      fprintf (stderr, "Failed to load public key.\n");
      return EXIT_FAILURE;
    }

  FILE *file = fopen (filename, "wb");
  if (!file)
    {
      perror ("Failed to open file for writing");
      exit (EXIT_FAILURE);
    }

  unsigned char *plaintext = malloc (key_len + iv_len);
  memcpy (plaintext, key, key_len);
  memcpy (plaintext + key_len, iv, iv_len);

  unsigned char encrypted[256];
  size_t encrypted_len;
  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new (pubkey, NULL);
  if (!ctx)
    handle_errors ();

  if (EVP_PKEY_encrypt_init (ctx) <= 0)
    handle_errors ();
  if (EVP_PKEY_encrypt (ctx, encrypted, &encrypted_len, plaintext,
                        key_len + iv_len)
      <= 0)
    handle_errors ();

  fwrite (encrypted, 1, encrypted_len, file);
  fclose (file);
  EVP_PKEY_CTX_free (ctx);
  EVP_PKEY_free (pubkey);
  free (plaintext);
  return EXIT_SUCCESS;
}

/*
        Load key and iv from a encrypted file
*/
size_t
load_decrypted_key_iv (const char *filename, unsigned char *key,
                       size_t key_len, unsigned char *iv, size_t iv_len,
                       const char *privatekey_file)
{
  EVP_PKEY *privatekey = load_private_key (privatekey_file);
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

  EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new (privatekey, NULL);
  if (!ctx)
    handle_errors ();

  if (EVP_PKEY_decrypt_init (ctx) <= 0)
    handle_errors ();
  if (EVP_PKEY_decrypt (ctx, plaintext, &plaintext_len, encrypted,
                        sizeof (encrypted))
      <= 0)
    handle_errors ();

  memcpy (key, plaintext, key_len);
  memcpy (iv, plaintext + key_len, iv_len);

  printf ("\nin function:");
  printf ("\nKEY = %s", key);
  printf ("\nIV = %s\n", iv);

  EVP_PKEY_CTX_free (ctx);
  EVP_PKEY_free (privatekey);
  free (plaintext);
  return EXIT_SUCCESS;
}
