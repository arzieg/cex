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

typedef struct Stack {
  int number;
  struct Stack *next;
} Stack_t;

bool createStack(Stack_t **s) {
  *s = NULL;
  return true;
}

bool pushStack(Stack_t **s, int n) {
  Stack_t *elem;
  elem = (Stack_t *)malloc(sizeof(Stack_t *));
  if (!elem) return false;
  elem->number = n;
  elem->next = *s;
  *s = elem;
  printf("\nPush %d to Stack on Positon %p, before is %p", elem->number, &elem,
         elem->next);
  return true;
}

bool isEmpty(Stack_t **s) {
  if (*s == NULL) return true;
  return false;
}

bool popStack(Stack_t **s, int *data) {
  Stack_t *elem;
  elem = *s;
  if (isEmpty(&elem)) return false;
  *s = elem->next;
  *data = elem->number;
  free(elem);
  return true;
}

bool destroyStack(Stack_t **s) {
  if (!*s) return true;
  destroyStack(&((*s)->next));
  free(*s);
  *s = NULL;
}

void displayNode(Stack_t *n) { printf("\tStack value:       %d\n", n->number); }

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
  pushStack(&s, a);
  a = 7;
  pushStack(&s, a);
  a = 9;
  pushStack(&s, a);
  printf("\n\n\nStack is\n");
  displayStack(&s);
  int data;
  popStack(&s, &data);
  printf("\nPOP from Stack, got %d\n", data);
  a = 11;
  pushStack(&s, a);
  printf("\n\n\nStack is\n");
  displayStack(&s);
  destroyStack(&s);
}
