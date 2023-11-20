/* Optimized

   Write a function canSum(targetSum, numbers) that takes in a
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

   Complexity:
   time = O(n^m)
   space = O(m)
   */

#include <stdio.h>
#include <stdbool.h>
#include <glib.h>

#define MAXSIZE 10

int array[MAXSIZE];

GHashTable *hash;

void printKeyValue(gpointer key, gpointer value, gpointer userData)
{
    int *realKey = key;
    bool realValue = *(bool *)value;

    printf("%d => %d\n", *realKey, realValue);

    return;
}

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
    gint *key = g_new(gint, 1);
    gboolean *value = g_new(gboolean, 1);

    *key = targetSum;

    if (g_hash_table_lookup(hash, key) != NULL)
        return *(gint *)g_hash_table_lookup(hash, key);
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
            *value = true;
            // printf("\nTRUE, insert key=%d, value=%d", *key, *value);
            g_hash_table_insert(hash, key, value);
            return true;
        }
    }
    *value = false;
    // printf("\nFALSE, insert key=%d, value=%d", *key, *value);
    g_hash_table_insert(hash, key, value);
    return false;
}

int main()
{
    hash = g_hash_table_new(g_int_hash, g_int_equal);
    array[0] = 2;
    array[1] = 3;
    printArray(7, array, 2);
    printf("%s\n", canSum(7, array, 2) ? "true" : "false");
    g_hash_table_foreach(hash, printKeyValue, NULL);
    g_hash_table_destroy(hash);

    hash = g_hash_table_new(g_int_hash, g_int_equal);
    array[0] = 5;
    array[1] = 3;
    array[2] = 4;
    array[3] = 7;
    printArray(7, array, 4);
    printf("%s\n", canSum(7, array, 4) ? "true" : "false");
    g_hash_table_foreach(hash, printKeyValue, NULL);
    g_hash_table_destroy(hash);

    hash = g_hash_table_new(g_int_hash, g_int_equal);
    array[0] = 2;
    array[1] = 4;
    printArray(7, array, 2);
    printf("%s\n", canSum(7, array, 2) ? "true" : "false");
    g_hash_table_foreach(hash, printKeyValue, NULL);
    g_hash_table_destroy(hash);

    hash = g_hash_table_new(g_int_hash, g_int_equal);
    array[0] = 2;
    array[1] = 3;
    array[2] = 5;
    printArray(8, array, 3);
    printf("%s\n", canSum(8, array, 3) ? "true" : "false");
    g_hash_table_foreach(hash, printKeyValue, NULL);
    g_hash_table_destroy(hash);

    hash = g_hash_table_new(g_int_hash, g_int_equal);
    array[0] = 7;
    array[1] = 14;
    printArray(300, array, 2);
    printf("%s\n", canSum(300, array, 2) ? "true" : "false");
    // g_hash_table_foreach(hash, printKeyValue, NULL);
    g_hash_table_destroy(hash);

    return 0;
}