#include <stdio.h>
#include <string.h>

/*
Input = [1,2,3,4]
Output = [1,4,3,8]
*/

void double_every_other(size_t length, int array[length]) {
  // mutate the array in-place
  for (int i = 1; i < length; i += 2) {
    printf("\ni=%d, value %d = ", i, array[i]);
    array[i] *= 2;
    printf("%d", array[i]);
  }
}

int main() {
  int number[] = {1, 2, 3, 4, 5};

  double_every_other(5, number);

  return 0;
}
