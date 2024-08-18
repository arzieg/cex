#include <stdio.h>

/*
2,2,2 --> 2
2,6,2 --> 12 (2 + 4 + 6)
1,5,1 --> 15 (1 + 2 + 3 + 4 + 5)
1,5,3  --> 5 (1 + 4)
*/

unsigned sequence_sum(unsigned start, unsigned end, unsigned step) {
  unsigned y = 0;
  if (start > end) return 0;

  for (unsigned i = start; i <= end; i += step) {
    y += i;
  }

  return y;
}

int main(void) {
  printf("\nTest 1: %d", sequence_sum(2, 2, 2));
  printf("\nTest 1: %d", sequence_sum(2, 6, 2));
  printf("\nTest 1: %d", sequence_sum(1, 5, 1));
  printf("\nTest 2: %d", sequence_sum(1, 5, 3));
  printf("\nTest 2: %d", sequence_sum(5, 1, 3));

  return 0;
}