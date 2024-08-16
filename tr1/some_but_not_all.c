#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
('abcdefg&%$', x -> isLetter(x)) == true
('&%$=', x -> isLetter x) == false
('abcdefg', x -> isLetter x) == false

([4, 1], x -> x > 3) == true
([1, 1], x -> x > 3) == false
([4, 4], x -> x > 3) == false
*/
#define ARR_LEN(array) (sizeof(array) / sizeof *(array))
typedef bool (*Predicate)(const void *);

static bool string_is_empty(const void *ptr) {
  const char *string = *(const char *const *)ptr;
  return *string == '\0';
}

static bool int_is_even(const void *ptr) { return *(const int *)ptr % 2 == 0; }

static bool triplet_is_ascending(const void *ptr) {
  const short(*triplet)[3] = ptr;
  return (*triplet)[0] < (*triplet)[1] && (*triplet)[1] < (*triplet)[2];
}

// length: length of the array
// size: size of an array element
// predicate: predicate on the array's elements
bool some_but_not_all(const void *array, size_t length, size_t size,
                      Predicate predicate) {
  int count = 0;

  for (size_t i = 0; i < length; i++) {
    if (predicate(array) == true) count++;
    array += size;
  }

  return ((count == 0) || (count == length)) ? false : true;
}

int main(void) {
  int ints[] = {5, 7, -15};
  char *strings[] = {"this", "is", "not", "empty"};
  short triplets[][3] = {{8, 7, 0}, {51, 54, 10}, {0, 1, 0}};

  bool test;
  test = some_but_not_all(ints, ARR_LEN(ints), sizeof *ints, int_is_even);
  if (test == true) {
    printf("\n Test ist true\n");
  } else {
    printf("\nTest ist false\n");
  }
  test = some_but_not_all(strings, ARR_LEN(strings), sizeof *strings,
                          string_is_empty);
  if (test == true) {
    printf("\n Test ist true\n");
  } else {
    printf("\nTest ist false\n");
  }
  test = some_but_not_all(triplets, ARR_LEN(triplets), sizeof *triplets,
                          triplet_is_ascending);
  if (test == true) {
    printf("\n Test ist true\n");
  } else {
    printf("\nTest ist false\n");
  }

  return 0;
}