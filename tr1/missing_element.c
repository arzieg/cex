#include <stdio.h>

short missing_element(const short elements[9]) {
  short rv = 0;

  for (int i = 0; i < 9; i++) {
    rv += elements[i];
  }
  return 45 - rv;
}

int main(void) {
  short a[] = {9, 2, 4, 5, 7, 0, 8, 6, 1};

  short missing = 0;

  missing = missing_element(a);
  printf("\nmissing = %d", missing);

  return 0;
}