#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long maxRot(long long n) {
  /*
  char rotate;
  char new_s[21];
  long long max = n;
  long long new_v = 0;
  sprintf(new_s, "%lld", n);
  int slen = strlen(new_s);
  int index = 0;
  char *eptr;

  while (index < slen - 1) {
    rotate = new_s[index];
    for (int j = index; j < slen - 1; j++) {
      new_s[j] = new_s[j + 1];
    }
    new_s[slen - 1] = rotate;
    printf("\nindex=%d, new_s=%s", index, new_s);
    index++;
    new_v = strtoll(new_s, &eptr, 10);
    if (new_v > max) max = new_v;
  }
  return max;
*/
  long long t = n, max = n, l = 10;

  while (t >= 10) l *= 10, t /= 10;

  while (l >= 10) {
    t = (n / l) * l +  // First digit(s)
        (n % (l / 10)) *
            10 +            // The last digits, moved one position to the left.
        (n * 10 / l) % 10;  // The rotating digit, moved to the last position.
    if (t > max) max = t;
    n = t, l /= 10;
    printf("\n n=%lld,  l=%lld, t=%lld", n, l, t);
  }

  return max;
}

int main(void) {
  long long number = 16130873362142;
  long long rv = 0;

  rv = maxRot(number);
  printf("\n\n%lld", rv);

  return 0;
}