/* time console */
#include <stdio.h>
#include <time.h>

int main(void) {
  time_t timer;
  time(&timer);
  printf("\nlocal time is: %s", ctime(&timer));
  return 0;
}
