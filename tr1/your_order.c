#include <malloc.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char *wordptr;
  int wordlength;
} WordStruct;

char *order_words(char *ordered, const char *words) {
  // write to ordered and return it
  // the size of ordered is equal to 1 + strlen(words)

  WordStruct word[10];

  // initialize
  for (int i = 0; i < 10; i++) {
    word[i].wordptr = NULL;
    word[i].wordlength = 0;
  }

  char *token;
  char *rest = (char *)malloc(strlen(words) * sizeof(char));
  strncpy(rest, words, strlen(words));

  while ((token = strsep(&rest, " ")) != NULL) {
    printf("\n %s", token);
    for (int i = 1; i < 10; i++) {
      char str[2];
      sprintf(str, "%d", i);
      char *myPtr = strstr(token, str);
      if (myPtr != NULL) {
        printf("\nFound: %s", str);
        word[i].wordptr = myPtr;
        word[i].wordlength = strlen(token);
        printf("  Ptr = %p", (void *)word[i].wordptr);
        printf("  Length = %d", word[i].wordlength);
      }
    }
  }

  printf("\n\n");

  for (int i = 1; i < 10; i++) {
    if (word[i].wordptr != NULL) {
      printf("\n Ptr = %p", (void *)word[i].wordptr);
      strncat(ordered, word[i].wordptr, word[i].wordlength);
      printf("  text = %s", ordered);
    }
  }

  *ordered = '\0';
  return ordered;
}

int main(void) {
  char *text = "is2 Th1s T4est 3a";
  char *res = (char *)malloc(50 * sizeof(char));

  order_words(res, text);
  printf("\n%s ", res);

  return 0;
}