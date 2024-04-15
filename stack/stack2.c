/*
 eine weitere Stack Implementierung
 */

// create
// isEmpty
// pop
// push
// peek

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Point {
  int x;
  int y;
} Point_t;

typedef struct Stack {
  Point_t p;
  struct Stack *next;
} Stack_t;

bool createStack(Stack_t **s) {
  *s = NULL;
  return true;
}

bool pushStack(Stack_t **s, int x, int y) {
  Stack_t *elem;
  elem = (Stack_t *)malloc(sizeof(Stack_t *));
  if (!elem) return false;
  elem->p.x = x;
  elem->p.y = y;
  elem->next = *s;
  *s = elem;
  printf("\nPush x=%d and y=%d to Stack on Positon %p, before address is %p",
         elem->p.x, elem->p.y, &elem, elem->next);
  return true;
}

bool isEmpty(Stack_t **s) {
  if (*s == NULL) return true;
  return false;
}

bool popStack(Stack_t **s, Point_t *data) {
  Stack_t *elem;
  elem = *s;
  if (isEmpty(&elem)) return false;
  *s = elem->next;
  data->x = elem->p.x;
  data->y = elem->p.y;
  free(elem);
  return true;
}

bool destroyStack(Stack_t **s) {
  if (!*s) return true;
  destroyStack(&((*s)->next));
  free(*s);
  *s = NULL;
}

void displayNode(Stack_t *n) {
  printf("\tStack value:       x=%d y=%d\n", n->p.x, n->p.y);
}

void displayStack(Stack_t **s) {
  for (Stack_t *cur = *s; cur != NULL; cur = cur->next) displayNode(cur);
}

size_t main() {
  Stack_t *s;
  if (!createStack(&s)) {
    fprintf(stderr, "Could not create stack");
    exit(EXIT_FAILURE);
  }

  int a = 5;
  int b = 6;
  pushStack(&s, a, b);
  a = 7;
  b = 8;
  pushStack(&s, a, b);
  a = 9;
  b = 10;
  pushStack(&s, a, b);
  printf("\n\n\nStack is\n");
  displayStack(&s);

  Point_t data;
  popStack(&s, &data);
  printf("\nPOP from Stack, got x=%d y=%d\n", data.x, data.y);
  a = 11;
  b = 12;
  pushStack(&s, a, b);
  printf("\n\n\nStack is\n");
  displayStack(&s);
  destroyStack(&s);
}
