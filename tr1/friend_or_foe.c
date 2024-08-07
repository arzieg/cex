#include <stdio.h>
#include <string.h>

/*
Input = ["Ryan", "Kieran", "Jason", "Yous"]
Output = ["Ryan", "Yous"]
*/

void filter_friends(const char *names[]) {
  // names[] is NULL-terminated, mutate it
  // You must re-use the pointers in 'names', do not allocate memory
  // Do not forget to NULL-terminate 'names'

  // laenge des arrays
  int i = 0, j = 0;
  while (names[i]) {
    if (strlen(names[i]) == 4) {
      names[j] = names[i];
      printf("%s\n", names[i]);
      j++;
    }
    i++;
  }
  names[j] = NULL;
}

int main() {
  const char *names[] = {"Ryan", "Kieran", "Jason", "Yous", NULL};

  filter_friends(names);

  return 0;
}
