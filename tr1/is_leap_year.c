#include <stdbool.h>
#include <stdio.h>

bool IsLeapYear(int year) {
  bool isleap = false;

  if (year < 1600 || year > 4000) return false;
  if (year % 400 == 0) isleap = true;
  if ((year % 4 == 0) && (year % 100 != 0)) isleap = true;
  return isleap;
}

int main(void) {
  int x = 1700;
  bool leapyear;

  leapyear = IsLeapYear(x);
  printf("Year %d is leapyear %d", x, leapyear);

  return 0;
}