#include <stdio.h>
#include <stdlib.h>

void
save_key_iv (const char *filename, unsigned char *key, unsigned char *iv)
{
  FILE *file = fopen (filename, "wb");
  if (!file)
    {
      perror ("Failed to open file for writing");
      exit (EXIT_FAILURE);
    }
  fwrite (key, 1, 32, file); // Save 256-bit key
  fwrite (iv, 1, 16, file);  // Save 128-bit IV
  fclose (file);
}

void
load_key_iv (const char *filename, unsigned char *key, unsigned char *iv)
{
  FILE *file = fopen (filename, "rb");
  if (!file)
    {
      perror ("Failed to open file for reading");
      exit (EXIT_FAILURE);
    }
  fread (key, 1, 32, file); // Load 256-bit key
  fread (iv, 1, 16, file);  // Load 128-bit IV
  fclose (file);
}

int
main ()
{
  unsigned char key[32]; // 256-bit key
  unsigned char iv[16];  // 128-bit IV

  // Assume key and iv are already generated
  save_key_iv ("key_iv.bin", key, iv);
  load_key_iv ("key_iv.bin", key, iv);

  return 0;
}
