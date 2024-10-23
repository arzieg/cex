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

  openssl pkeyutl -encrypt -pubin -inkey key.pub -in plaintext -out
encrypted.bin

Decrypt file:

  openssl pkeyutl -decrypt -inkey private.pem -in encrypted.bin -out
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

/*
 gernerate privkey / pubkey pair
*/
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

/*
 save key and initialization vector in an encrypted file
*/

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

/*
 load key and initialization vector from an encrypted file
*/
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

int
do_crypt (const char *filein, const char *fileout, int do_encrypt,
          unsigned char *key, unsigned char *iv)
{
  /* Allow enough space in output buffer for additional block */
  unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
  int inlen, outlen;
  EVP_CIPHER_CTX *ctx;
  /*
   * Bogus key and IV: we'd normally set these from
   * another source.
   */
  // unsigned char key[] = "0123456789abcdeF0123456789abcdeF";
  // unsigned char iv[] = "1234567887654321";

  /* Don't set key or IV right away; we want to check lengths */
  ctx = EVP_CIPHER_CTX_new ();
  if (!EVP_CipherInit_ex2 (ctx, EVP_aes_256_cbc (), NULL, NULL, do_encrypt,
                           NULL))
    {
      /* Error */
      EVP_CIPHER_CTX_free (ctx);
      return 0;
    }

  OPENSSL_assert (EVP_CIPHER_CTX_get_key_length (ctx) == 32);
  OPENSSL_assert (EVP_CIPHER_CTX_get_iv_length (ctx) == 16);

  /* Now we can set key and IV */
  if (!EVP_CipherInit_ex2 (ctx, NULL, key, iv, do_encrypt, NULL))
    {
      /* Error */
      EVP_CIPHER_CTX_free (ctx);
      return 0;
    }

  /* open file */
  FILE *in = fopen (filein, "r");
  if (in == NULL)
    {
      fprintf (stderr, "Error opening %s \n", filein);
      return (EXIT_FAILURE);
    }
  FILE *out = fopen (fileout, "w");
  if (out == NULL)
    {
      fprintf (stderr, "Error opening %s \n", fileout);
      return (EXIT_FAILURE);
    }

  for (;;)
    {
      inlen = (int)fread (inbuf, 1, 1024, in);
      if (inlen <= 0)
        break;
      if (!EVP_CipherUpdate (ctx, outbuf, &outlen, inbuf, inlen))
        {
          /* Error */
          EVP_CIPHER_CTX_free (ctx);
          return 0;
        }
      fwrite (outbuf, 1, (size_t)outlen, out);
    }
  if (!EVP_CipherFinal_ex (ctx, outbuf, &outlen))
    {
      /* Error */
      EVP_CIPHER_CTX_free (ctx);
      return 0;
    }
  fwrite (outbuf, 1, (size_t)outlen, out);

  EVP_CIPHER_CTX_free (ctx);
  return 1;
}