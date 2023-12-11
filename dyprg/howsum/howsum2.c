/*
write a function "howSum(targetSum, numbers)" that takes in a
targetSum and an array of numbers as arguments.

The function should return an array containing any combinations of elements
that add up to exactly the targetSum. If there is no combination that adds up to
the targetSum, then return null.

If there are multiple combinations possible, you may return any single one.

Test:
------
howSum(7, [2,3]) -> [3,2,2]
howSum(7, [5,3,4,7]) -> [4,3]
howSum(7, [2,4]) -> null
howSum(8, [2,3,5]) -> [2,2,2,2]
howSum(300, [7,14]) -> null

Complexity:
  time = O(n^m * m) *m wg. copy der Arrays mit gefundenen Lösungen je Durchlauf
  space = O(m)


result = glist 3->2->2
*/

#include <glib.h>
#include <stdbool.h>
#include <stdio.h>

#define MAXSIZE 10
GSList *result = NULL;  // list of results

void print_iterator(gpointer item) { printf("%d\n", GPOINTER_TO_INT(item)); }

int *howSum(int targetSum, GSList array, gpointer result) {
  if (targetSum == 0) {
    printf("\ntargetSum == 0\n");
    g_slist_foreach(result, (GFunc)print_iterator, NULL);
    return result;
  }

  if (targetSum < 0) return NULL;

  // 7 [2,3]   [3,2,2]

  GSList *num = NULL;
  int remainder = 0;

  for (num = array; num; num = num->next) {
    int numdata = GPOINTER_TO_INT(num->data);
    remainder = targetSum - numdata;
    printf("\nremainder = targetSum - numdata : %d = %d - %d", remainder,
           targetSum, numdata);

    int *remainderResult = howSum(remainder, array, result);
    if (remainderResult != NULL) {
      printf("\nReminder Result %d", *(int *)remainderResult);
      result = g_slist_append(result, *(int *)remainderResult);
      return result;
    }
  }
  printf("\nRETURN NULL\n");
  return NULL;
}

int main() {
  GSList *result = NULL;  // list of results
  GSList *array = NULL;   // list of available numbers

  // initialize array and print
  array = g_slist_append(array, (int *)2);
  array = g_slist_append(array, (int *)3);
  result = g_slist_append(result, (int *)5);
  g_slist_foreach(array, (GFunc)print_iterator, NULL);
  result = howSum(7, array, result);
  printf("\nPRINT RESULT IN MAIN\n");
  g_slist_foreach(result, (GFunc)print_iterator, NULL);
  /*
    printf("\nhowSum 7 of array [");

    if (howSum(7, array, 2) == 0) {
      printf("] is [");
      printArray2();
    } else {
      printf("] has no solution.");
    }
  */
  // resultcnt = 0;
  // array[0] = 5;
  // array[1] = 3;
  // array[2] = 4;
  // array[3] = 7;
  // printf("\nhowSum 7 of array [");
  // printArray(7, array, 4);
  // howSum(7, array, 4);
  // printf("] is [");
  // printArray(7, result, resultcnt);
  // printf("]\n\n");

  // resultcnt = 0;
  // array[0] = 2;
  // array[1] = 4;
  // printf("\nhowSum 7 of array [");
  // printArray(7, array, 2);
  // howSum(7, array, 2);
  // printf("] is [");
  // printArray(7, result, resultcnt);
  // printf("]\n\n");

  // resultcnt = 0;
  // array[0] = 2;
  // array[1] = 3;
  // array[2] = 5;
  // printf("\nhowSum 8 of array [");
  // printArray(8, array, 3);
  // howSum(8, array, 3);
  // printf("] is [");
  // printArray(7, result, resultcnt);
  // printf("]\n\n");

  // resultcnt = 0;
  // array[0] = 7;
  // array[1] = 14;
  // printf("\nhowSum 300 of array [");
  // printArray(300, array, 2);
  // howSum(300, array, 2);
  // printf("] is [");
  // printArray(300, result, resultcnt);
  // printf("]\n\n");

  return 0;
}