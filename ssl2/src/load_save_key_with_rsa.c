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

void
handle_errors ()
{
  ERR_print_errors_fp (stderr);
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

void
save_encrypted_key_iv (const char *filename, unsigned char *key,
                       size_t key_len, unsigned char *iv, size_t iv_len,
                       EVP_PKEY *pubkey)
{
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
  EVP_PKEY_CTX_free (ctx);
  free (plaintext);
}

void
load_decrypted_key_iv (const char *filename, unsigned char *key,
                       size_t key_len, unsigned char *iv, size_t iv_len,
                       EVP_PKEY *privkey)
{
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

  EVP_PKEY_CTX_free (ctx);
  free (plaintext);
}

int
main ()
{
  unsigned char key[32]; // 256-bit key
  unsigned char iv[16];  // 128-bit IV

  // Generate RSA keys
  generate_rsa_keys ("public.pem", "private.pem");

  // Generate random key and IV
  if (!RAND_bytes (key, sizeof (key)) || !RAND_bytes (iv, sizeof (iv)))
    {
      fprintf (stderr, "Error generating random bytes.\n");
      return 1;
    }

  // Load RSA public key
  FILE *pubkey_file = fopen ("public.pem", "r");
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
      return 1;
    }

  // Save encrypted key and IV
  save_encrypted_key_iv ("key_iv_encrypted.bin", key, sizeof (key), iv,
                         sizeof (iv), pubkey);

  EVP_PKEY_free (pubkey);

  // Load RSA private key
  FILE *privkey_file = fopen ("private.pem", "r");
  if (!privkey_file)
    {
      perror ("Unable to open private key file");
      return 1;
    }
  EVP_PKEY *privkey = PEM_read_PrivateKey (privkey_file, NULL, NULL, NULL);
  fclose (privkey_file);

  if (!privkey)
    {
      fprintf (stderr, "Error reading private key.\n");
      return 1;
    }

  unsigned char decrypted_key[32];
  unsigned char decrypted_iv[16];

  // Load and decrypt key and IV
  load_decrypted_key_iv ("key_iv_encrypted.bin", decrypted_key,
                         sizeof (decrypted_key), decrypted_iv,
                         sizeof (decrypted_iv), privkey);

  EVP_PKEY_free (privkey);

  // Now you can use the decrypted key and IV for your encryption tasks
  printf ("Key and IV successfully decrypted and loaded.\n");

  return 0;
}
