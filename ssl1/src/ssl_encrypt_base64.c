/*
  Basic Input Output openssl (BIO-Function)
  https://www.youtube.com/watch?v=kxAdd03wyUo

  Beispiel für eine base64 codierung

  Aufruf: bin/ssl_encode_base64 <inputfile> <base64outfile>

*/
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <stdio.h>

#define MAX_BUFFER_SIZE 4096

int
main (int argc, char *argv[])
{
  BIO *bio_out = NULL;
  BIO *bio_in = NULL;
  BIO *bio_b64 = NULL;

  int inByte, outByte = 0;
  char buffer[MAX_BUFFER_SIZE];

  memset (buffer, '\0', MAX_BUFFER_SIZE);

  if (argc < 3)
    {
      fprintf (stderr, "Usage sshbio <infile> <outfile>");
      exit (EXIT_FAILURE);
    }

  bio_in = BIO_new_file (argv[1], "r");
  bio_out = BIO_new_file (argv[2], "wb");
  bio_b64 = BIO_new (BIO_f_base64 ());
  BIO_push (bio_b64, bio_out);

  while ((inByte = BIO_read (bio_in, buffer, MAX_BUFFER_SIZE)) > 0)
    {
      outByte = BIO_write (bio_b64, buffer, inByte);
      if (inByte != outByte)
        {
          fprintf (stderr, "inByte %d != outByte %d", inByte, outByte);
          break;
        }
    }
  BIO_flush (bio_b64);
  BIO_free (bio_in);
  BIO_free (bio_out);
  BIO_free (bio_b64);

  exit (EXIT_SUCCESS);
}