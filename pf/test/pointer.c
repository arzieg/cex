#include <stdio.h>

int a = 5;

int *b;

void func(int *c) {
  printf("\nc=%d", *c);
  (*c)++;

  printf("\nc=%d", *c);
}

int main() {
  b = &a;
  func(b);
  printf("\na=%d", a);
  printf("\nb=%d\n", *b);
  return 0;
}