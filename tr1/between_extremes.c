#include <stddef.h>
#include <stdio.h>

/*
[23, 3, 19, 21, 16] should return 20 (i.e., 23 - 3).

[1, 434, 555, 34, 112] should return 554 (i.e., 555 - 1).
*/

int between_extremes(size_t length, const int numbers[length]) {
  int max = 0, min = 0;

  if (length > 1) {
    max = numbers[length - 1];
    min = numbers[length - 2];
  } else {
    return numbers[length];
  }

  for (size_t i = 0; i < length; i++) {
    if (numbers[i] > max) max = numbers[i];
    if (numbers[i] < min) min = numbers[i];
  }

  return max - min;
}

int main(void) {
  // int a[] = {23, 3, 19, 21, 16};
  // int a[] = {1, 434, 555, 34, 112};
  int a[] = {-1, -1};
  printf("\n%d\n", between_extremes(2, a));
  return 0;
}