#include <stdbool.h>
#include <stdio.h>

/*
 Get three values
 check if
  a < b+c
  b < a+c
  c < a+b
*/

bool is_triangle(int a, int b, int c) {
  return ((a < b + c) && (b < a + c) && (c < a + b)) ? true : false;
}

int main(void) {
  printf("\nTest 1 %d", is_triangle(4, 2, 3));
  printf("\nTest 2 %d", is_triangle(-5, 1, 3));
}