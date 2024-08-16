#include <math.h>
#include <stdbool.h>
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

void printBits(unsigned long int num) {
  // printf("\n%d:    ", num);
  for (int bit = 0; bit < (sizeof(unsigned int) * 8); bit++) {
    printf("%li", num & 0x01);
    num = num >> 1;
  }
}

void dezimalZuBinaer(int n) {
  if (n > 1) {
    dezimalZuBinaer(n / 2);
  }
  printf("%d", n % 2);
}

int main(void) {
  // int position = 133;
  char *r = "x  ox x x   xo oxo     o o   o ox xox x";

  long int position = 0;
  long int y = 0;
  long int inv = 0;
  long int alwaysdark = 0;

  int rooms = 0;

  while (r[rooms] != '\0') {
    printf("%c", r[rooms]);
    if (r[rooms] == 'x' || r[rooms] == ' ') position += pow(2, rooms);
    if (r[rooms] == ' ') alwaysdark += pow(2, rooms);
    rooms++;
  }

  printf("\nroomdec = %li", position);
  printf("\nalways dark = %li", alwaysdark);
  printf("\nrooms = %d", rooms);

  while (((position & 1) == 1) && (rooms > 0)) {
    printf("\nroom %d", rooms);
    printf("\nposition = ");
    printBits(position);
    printf("\nalwaysda = ");
    printBits(alwaysdark);
    inv = ~position;  // inverse
    printf("\ninv      = ");
    printBits(inv);
    position = inv >> 1;  // shift right
    alwaysdark >>= 1;     // shift right
    position |= alwaysdark;
    printf("\nposition = ");
    printBits(position);
    printf("\nalwaysda = ");
    printBits(alwaysdark);

    y = position & 1;
    printf("\ny        = ");
    printBits(y);

    printf("\n\n");
    rooms--;
  }

  if (rooms == 0) {
    printf("\nHurra geschafft\n");
  } else {
    printf("\ndas war nichts\n");
  }

  return 0;
}

/*
bool bulb_maze(const char *m) {
  for (int i = 0; m[i]; i++)
    if (m[i] == (i % 2 ? 'x' : 'o')) return false;
  return true;
}

int main(void) {
  // char *r = "x  ox x x   xo oxo     o o   o ox xox x";
  char *r = "xo oxox";

  bool rooms = bulb_maze(r);

  if (rooms == 1) {
    printf("\nHurra geschafft\n");
  } else {
    printf("\ndas war nichts\n");
  }

  return 0;
}
*/