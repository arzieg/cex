/* Write a function canSum(targetSum, numbers) that takes in a
   targetSum and an array of numbers as arguments

   The function should return a boolean indicating whether or not it
   is possible to generate the targetSum using numbers from the array

   you may use an element of the array as many times as needed

   you may assume that all input numbers are non-negative

   canSum(7, [5,3,4,7]) -> true (3,4)(7)
   canSum(7, [2,4]) -> false

   Test:
   ------
   canSum(7, [2,3]) -> true
   canSum(7, [5,3,4,7]) -> true
   canSum(7, [2,4]) -> false
   canSum(8, [2,3,5]) -> true
   canSum(300, [7,14]) -> false
   */

#include <stdio.h>
#include <stdbool.h>

#define MAXSIZE 10

int array[MAXSIZE];

void printArray(int targetSum, int *array, int maxvalues)
{
    int i = 0;
    printf("\ncanSum %d of array [", targetSum);
    for (i = 0; i < maxvalues; i++)
        printf("%d,", array[i]);
    printf("] is ");
    return;
}

bool canSum(int targetSum, int *array, int maxvalues)
{
    if (targetSum == 0)
        return true;
    if (targetSum < 0)
        return false;

    int num = 0;
    int remainder = 0;
    for (num = 0; num < maxvalues; num++)
    {
        remainder = targetSum - array[num];
        if (canSum(remainder, array, maxvalues) == true)
        {
            return true;
        }
    }
    return false;
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