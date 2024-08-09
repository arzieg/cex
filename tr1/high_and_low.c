#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void high_and_low(const char *strnum, char *result) {
  //   // print your answer to <result> :
  //   int max = 0, min = 0;
  //   int slen = strlen(strnum);
  //   char s[slen];
  //   char buffer[100];

  //   for (int i = 0; i < slen; i++) {
  //     s[i] = strnum[i];
  //   }
  //   char *token = strtok(s, " ");
  //   min = atoi(token);
  //   max = atoi(token);

  //   while (token != NULL) {
  //     if (atoi(token) > max) max = atoi(token);
  //     if (atoi(token) < min) min = atoi(token);
  //     token = strtok(NULL, " ");
  //   }
  //   sprintf(buffer, "%d %d", max, min);
  //   printf(buffer);
  int min, max;
  char buffer[100];

  min = max = atoi(strnum);
  for (int i = 0; strnum[i]; i++) {
    int tmp;
    if (strnum[i] == ' ') {
      tmp = atoi(strnum + i);
      if (tmp < min) min = tmp;
      if (tmp > max) max = tmp;
    }
  }
  sprintf(buffer, "%d %d", max, min);
  printf(buffer);
}

int main(void) {
  char *strnum = "1 -4 3 7 5";
  char *result = "5 1";

  high_and_low(strnum, result);

  return 0;
}