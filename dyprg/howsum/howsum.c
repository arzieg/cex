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

*/

#include <stdio.h>
#include <stdbool.h>

#define MAXSIZE 10
#define MAXSIZE_RESULT 300

int array[MAXSIZE];
int result[MAXSIZE_RESULT];
int resultcnt = 0;

void printArray(int targetSum, int *array, int maxvalues)
{
    int i = 0;
    for (i = 0; i < maxvalues; i++)
        printf("%d,", array[i]);
    return;
}

int *howSum(int targetSum, int *array, int maxvalues)
{
    if (targetSum == 0)
        return result;
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
            result[resultcnt] = array[num];
            resultcnt++;
            return result;
        }
    }
    return NULL;
}

int main()
{

    int array[MAXSIZE] = {2, 3};
    printf("\nhowSum 7 of array [");
    printArray(7, array, 2);
    howSum(7, array, 2);
    printf("] is [");
    printArray(7, result, resultcnt);
    printf("]\n\n");

    resultcnt = 0;
    array[0] = 5;
    array[1] = 3;
    array[2] = 4;
    array[3] = 7;
    printf("\nhowSum 7 of array [");
    printArray(7, array, 4);
    howSum(7, array, 4);
    printf("] is [");
    printArray(7, result, resultcnt);
    printf("]\n\n");

    resultcnt = 0;
    array[0] = 2;
    array[1] = 4;
    printf("\nhowSum 7 of array [");
    printArray(7, array, 2);
    howSum(7, array, 2);
    printf("] is [");
    printArray(7, result, resultcnt);
    printf("]\n\n");

    resultcnt = 0;
    array[0] = 2;
    array[1] = 3;
    array[2] = 5;
    printf("\nhowSum 8 of array [");
    printArray(8, array, 3);
    howSum(8, array, 3);
    printf("] is [");
    printArray(7, result, resultcnt);
    printf("]\n\n");

    resultcnt = 0;
    array[0] = 7;
    array[1] = 14;
    printf("\nhowSum 300 of array [");
    printArray(300, array, 2);
    howSum(300, array, 2);
    printf("] is [");
    printArray(300, result, resultcnt);
    printf("]\n\n");

    return 0;
}