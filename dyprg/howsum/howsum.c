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



*/

#include <stdio.h>
#include <stdbool.h>

#define MAXSIZE 10

int array[MAXSIZE];

void printArray(int targetSum, int *array, int maxvalues)
{
    int i = 0;
    printf("\nhowSum %d of array [", targetSum);
    for (i = 0; i < maxvalues; i++)
        printf("%d,", array[i]);
    printf("] is ");
    return;
}

bool howSum(int targetSum, int *array, int maxvalues)
{
    if (targetSum == 0)
        return *array;
    if (targetSum < 0)
        return NULL;

    int num = 0;
    int remainder = 0;
    for (num = 0; num < maxvalues; num++)
    {
        remainder = targetSum - array[num];
        remainderResult = howSum(remainder, array, maxvalues);
        if (remainderResult != NULL)
        {
            return [... remainderResult, array[num] ]; // array
        }
    }
    return NULL;
}

int main()
{
    int array[MAXSIZE] = {2, 3};
    printArray(7, array, 2);
    printf("%s\n", canSum(7, array, 2) ? "true" : "false");

    array[0] = 5;
    array[1] = 3;
    array[2] = 4;
    array[3] = 7;
    printArray(7, array, 4);
    printf("%s\n", canSum(7, array, 4) ? "true" : "false");

    array[0] = 2;
    array[1] = 4;
    printArray(7, array, 2);
    printf("%s\n", canSum(7, array, 2) ? "true" : "false");

    array[0] = 2;
    array[1] = 3;
    array[2] = 5;
    printArray(8, array, 3);
    printf("%s\n", canSum(8, array, 3) ? "true" : "false");

    array[0] = 7;
    array[1] = 14;
    printArray(300, array, 2);
    printf("%s\n", canSum(300, array, 2) ? "true" : "false");

    return 0;
}