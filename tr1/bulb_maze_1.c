#include <math.h>
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

void dezimalZuBinaer(int n) {
  if (n > 1) {
    dezimalZuBinaer(n / 2);
  }
  printf("%d", n % 2);
}

int main(void) {
  // int j = 133;
  char *r = "xoxoxxxo";
  int j = 0;
  int x = 0;
  int y = 0;
  int inv = 0;

  int rooms = 0;

  while (r[rooms] != '\0') {
    printf("%c", r[rooms]);
    if (r[rooms] == 'x' || r[rooms] == ' ') j += pow(2, rooms);
    rooms++;
  }

  printf("\nroomdec = %d", j);
  printf("\nrooms = %d", rooms);

  while (((j & 1) == 1) && (rooms > 0)) {
    printf("\nroom %d", rooms);
    printf("\nj = ");
    dezimalZuBinaer(j);
    inv = ~j;  // inverse
    printf("\nx= ");
    dezimalZuBinaer(x);
    j = inv >> 1;  // shift right
    y = j & 1;
    printf("\ny = ");
    dezimalZuBinaer(y);
    printf("\n\n\n\n");
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
bool bulb_maze (const char *maze)
{
  int j = 0;
  int inv = 0;
  int rooms = 0;

  while (maze[rooms] != '\0') {
    if (maze[rooms] == 'x' || maze[rooms] == ' ') j += pow(2, rooms);
    rooms++;
  }

  while (((j & 1) == 1) && (rooms > 0)) {
    inv = ~j;  // inverse
    j = inv >> 1;  // shift right
    rooms--;
  }
        return rooms == 0 ? true : false;
}
*/