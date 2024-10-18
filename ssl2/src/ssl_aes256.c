#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>

int do_crypt (FILE *in, FILE *out, int do_encrypt, unsigned char *iv);

int
do_crypt (FILE *in, FILE *out, int do_encrypt, unsigned char *iv)
{
  /* Allow enough space in output buffer for additional block */
  unsigned char inbuf[1024], outbuf[1024 + EVP_MAX_BLOCK_LENGTH];
  int inlen, outlen;
  EVP_CIPHER_CTX *ctx;
  /*
   * Bogus key and IV: we'd normally set these from
   * another source.
   */
  unsigned char key[] = "0123456789abcdeF0123456789abcdeF";
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

int
main (int argc, char *argv[])
{

  unsigned char iv[16]; // 128-bit IV

  int encrypt = 1;
  if (argc < 3)
    {
      fprintf (stderr,
               "Usage ssl_aes256 <infile> <outfile> <encrypt|decrypt> <iv>");
      exit (EXIT_FAILURE);
    }

  FILE *filein = fopen (argv[1], "r");
  if (filein == NULL)
    {
      fprintf (stderr, "Error opening %s \n", argv[1]);
      exit (EXIT_FAILURE);
    }
  FILE *fileout = fopen (argv[2], "w");
  if (fileout == NULL)
    {
      fprintf (stderr, "Error opening %s \n", argv[2]);
      exit (EXIT_FAILURE);
    }

  if (strcmp (argv[3], "encrypt") == 0)
    {
      encrypt = 1;

      if (RAND_bytes (iv, sizeof (iv)) != 1)
        {
          fprintf (stderr, "Error generating random bytes.\n");
          return 1;
        }

      printf ("Generated IV: ");
      for (unsigned int i = 0; i < sizeof (iv); i++)
        {
          printf ("%02x", iv[i]);
        }
      printf ("\n");
    }
  else if (strcmp (argv[3], "decrypt") == 0)
    {
      encrypt = 0;
      if (strlen (argv[4]) != 32)
        {
          fprintf (stderr, "Invalid hex string length.\n");
          return 1;
        }

      // Convert the hex string to an unsigned char array
      for (int i = 0; i < 16; i++)
        {
          sscanf (argv[4] + 2 * i, "%2hhx", &iv[i]);
        }
    }
  else
    {
      fprintf (stderr, "Please choose encrypt or decrypt");
      exit (EXIT_FAILURE);
    }

  do_crypt (filein, fileout, encrypt, iv);
  exit (EXIT_SUCCESS);
}