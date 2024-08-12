#include <stdio.h>
/*

x=light switch off
o=light switch on
 = light switch off

TRUE

step 0 :  xo oxox
          ^ <--------You are here
step 1 :  ox xoxo
           ^ <--------You are here
step 2 :  xo oxox
            ^ <--------You are here
step 3 :  ox xoxo
             ^ <--------You are here
step 4 :  xo oxox
              ^ <--------You are here
step 5 :  ox xoxo
               ^ <--------You are here
step 6 :  xo oxox
                ^ <--------You are here
step 7 :  ox xoxo
                 ^ <--------You've go through the maze :)


FALSE
step 0 :  xo  oxox
          ^ <--------You are here
step 1 :  ox  xoxo
           ^ <--------You are here
step 2 :  xo  oxox
            ^ <--------You are here
step 3 :  ox  xoxo
             ^ <--------You are here
step 4 :  xo  oxox
              ^ <--------You were caught by the enemy :(

*/

void printBits(unsigned int num) {
  printf("\n%d:    ", num);
  for (int bit = 0; bit < (sizeof(unsigned int) * 8); bit++) {
    printf("%i ", num & 0x01);
    num = num >> 1;
  }
}

int main(void) {
  int j = 133;
  int x = 128;
  int y = 0;

  while ((j & 1) == 1) {
    printBits(j);
    x = ~j;  // inverse
    printBits(x);
    j = x >> 1;  // shift right
  }

  return 0;
}
