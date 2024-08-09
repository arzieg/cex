#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long maxRot(long long n) {
  char s[sizeof(long long)];
  long long max = n;
  int slen = strlen(s);
  int i = 0, j = 0;
  char tmp;

  sprintf(s, "%lld", n);

  while (i < slen) {
    j = i + 1;
    tmp = s[j];
    for (; j < slen; j++) {
    }
  }

  // shift left

  //  itoa(num, snum, 10); int to string
  // atoi ; string to int

  return n;
}

int main(void) {
  long long number = 56789;
  long long rv = 0;

  rv = maxRot(number);
  printf("%lld", rv);

  return 0;
}