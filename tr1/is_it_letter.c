#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

bool is_it_letter(char c) { return isalpha(c); }

int main(void) {
  char c = '1';
  printf("A is a character %d", is_it_letter(c));
  return 0;
}