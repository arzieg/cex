#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <stdio.h>
#include <string.h>
/*
 ToDo:
   iv ist mit filename in eine Protokolldatei zuschreiben
   key ist zu übergeben
   iv ist dann als letztes Argument zu übergeben, da es nur für decrypt
 benötigt wird
*/

int do_crypt (FILE *in, FILE *out, int do_encrypt, unsigned char *key,
              unsigned char *iv);
void create_iv (unsigned char *iv, size_t size);
int hexStringToBytes (const char *hexString, unsigned char *iv,
                      size_t ivArraySize);

void
create_iv (unsigned char *iv, size_t size)
{
  if (RAND_bytes (iv, (int)size) != 1)
    {
      fprintf (stderr, "Error generating random bytes\n");
      exit (EXIT_FAILURE);
    }
  printf ("Generated IV: ");
  for (unsigned int i = 0; i < size; i++)
    {
      printf ("%02x", iv[i]);
    }
  printf ("\n");
}

int
hexStringToBytes (const char *hexString, unsigned char *iv, size_t ivArraySize)
{
  size_t hexStringLength = strlen (hexString);

  // Check if the hex string length is twice the byte array size
  if (hexStringLength != ivArraySize * 2)
    {
      fprintf (stderr,
               "Hex string length does not match the expected size.\n");
      return EXIT_FAILURE;
    }

  for (size_t i = 0; i < ivArraySize; i++)
    {
      // Convert each pair of hex digits to a byte
      if (sscanf (hexString + 2 * i, "%2hhx", &iv[i]) != 1)
        {
          fprintf (stderr, "Invalid hex string format.\n");
          return EXIT_FAILURE;
        }
    }

  return EXIT_SUCCESS;
}

int
do_crypt (FILE *in, FILE *out, int do_encrypt, unsigned char *key,
          unsigned char *iv)
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

  unsigned char iv[16];  // 128-bit IV
  unsigned char key[32]; // 256 BIT AES

  int encrypt = 1;
  if (argc < 3)
    {
      fprintf (
          stderr,
          "Usage ssl_aes256 <infile> <outfile> <encrypt|decrypt> <key> <iv>");
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
      create_iv (iv, sizeof (iv));
    }
  else if (strcmp (argv[3], "decrypt") == 0)
    {
      encrypt = 0;
      hexStringToBytes (argv[5], iv, sizeof (iv));
    }
  else
    {
      fprintf (stderr, "Please choose encrypt or decrypt");
      exit (EXIT_FAILURE);
    }

  memcpy (key, argv[4], 32);

  do_crypt (filein, fileout, encrypt, key, iv);
  exit (EXIT_SUCCESS);
}