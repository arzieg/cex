#include <stdio.h>

void double_swap(double* p0, double* p1, double* p2) {
  double tmp = *p0;
  *p0 = *p2;
  *p2 = tmp;
}

double sum0(size_t len, double const* a) {
  double ret = 0.0;
  for (size_t i = 0; i < len; ++i) {
    ret += *(a + i);
  }
  return ret;
}

int main() {
  double d0, d1, d2;
  d0 = 5;
  d1 = 7;
  d2 = 9;
  printf("\nd0=%f, d1=%f, d2=%f", d0, d1, d2);
  double_swap(&d0, &d1, &d2);
  printf("\nd0=%f, d1=%f, d2=%f\n", d0, d1, d2);

  printf("\nSum of an array");
  double A[7] = {
      0, 1, 2, 3, 4, 5, 6,
  };
  printf("\nSum of Array = %f\n", sum0(7, &A[0]));

  return 0;
}