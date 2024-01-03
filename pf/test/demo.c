#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  ssize_t length;
  char *string;
  size_t buffer_size;
} FancyString;

void FancyString_free(FancyString *target) {
  if (target->string) {
    free(target->string);
  }
  free(target);
}

FancyString *fancy_getline(FILE *stream) {
  FancyString *new = malloc(sizeof(*new));
  new->string = NULL;
  new->buffer_size = 0;

  new->length = getline(&(new->string), &(new->buffer_size), stream);
  if (new->length == -1) {
    free(new);
    return NULL;
  } else {
    return new;
  }
}

int main() {
  char text[2][4];

  for (int i = 0; i < 2; i++) {
    FancyString *line = fancy_getline(stdin);
    strncpy(text[i], line->string, line->length);

    printf("Read %zd bytes, buffer is %zd bytes\n", line->length,
           line->buffer_size);
    printf("Line read:\n%s", line->string);

    FancyString_free(line);
  }
  for (int i = 0; i < 2; i++) {
    printf("\nString %d = %s", i, text[i]);
  }
  return 0;
}