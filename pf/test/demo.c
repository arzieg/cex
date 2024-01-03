#define _GNU_SOURCE
#include <stdio.h>

int main() {
  char *string = NULL;
  size_t buffer_size = 0;
  ssize_t read_size;

  printf("Enter some stuff!\n");
  read_size = getline(&string, &buffer_size, stdin);

  printf("Read %zd bytes, buffer is %zd bytes\n", read_size, buffer_size);
  printf("Line read:\n%s", string);

  return 0;
}