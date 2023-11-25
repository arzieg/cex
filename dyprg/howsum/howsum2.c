/*
write a function "howSum(targetSum, numbers)" that takes in a
targetSum and an array of numbers as arguments.

The function should return an array containing any combinations of elements
that add up to exactly the targetSum. If there is no combination that adds up to the
targetSum, then return null.

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


hash
 key=string - value = 7
 2,2,3 = 7

*/

#include <stdio.h>
#include <stdbool.h>
#include <glib.h>

#define MAXSIZE 10

int array[MAXSIZE];
GSList *value;

void printArray(int targetSum, int *array, int maxvalues)
{
    int i = 0;
    for (i = 0; i < maxvalues; i++)
        printf("%d,", array[i]);
    return;
}

void printArray2()
{
    GSList *iterator = NULL;
    for (iterator = value; iterator; iterator = value->next)
    {
        printf("\nCurrent item is '%d'\n", *(int *)value->data);
    }
    return;
}

int *howSum(int targetSum, int *array, int maxvalues)
{
    gint *tmpval = g_new(gint, 1);

    if (targetSum == 0)
        return value;
    if (targetSum < 0)
        return NULL;

    int num = 0;
    int remainder = 0;
    for (num = 0; num < maxvalues; num++)
    {
        remainder = targetSum - array[num];

        int *remainderResult = howSum(remainder, array, maxvalues);
        if (remainderResult != NULL)
        {
            printf("Reminder Result !=0");
            *tmpval = remainderResult;
            g_slist_append(value, tmpval);
            return remainderResult;
        }
    }
    return NULL;
}

int main()
{
    GSList *value = NULL; // list of values

    int array[MAXSIZE] = {2, 3};
    printf("\nhowSum 7 of array [");
    printArray(7, array, 2);
    if (howSum(7, array, 2) == 0)
    {
        printf("] is [");
        printArray2();
    }
    else
    {
        printf("] has no solution.");
    }

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