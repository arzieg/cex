#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int calculate(int number) {
  int sum = 0;
  int i = number - 1;
  float j = 0;
  for (i; i > 0; i--) {
    printf("i=%d   sum=%d\n", i, sum);
    j = i / 3.0f;
    if (j == abs(j)) sum += i;

    j = i / 5.0f;
    if (j == abs(j)) sum += i;
  }
  return sum;
}

int main() {
  int result = 0;

  result = calculate(4);
  assert(result == 3);

  result = calculate(10);
  assert(result == 23);

  return 0;
}