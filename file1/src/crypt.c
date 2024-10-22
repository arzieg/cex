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
