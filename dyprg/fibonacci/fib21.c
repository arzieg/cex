// Fibonacci Brute Force
// Beispiel

// Time Complexity O(2^n)
// Space Complexity O(n)

#include <stdio.h>
#include <stdint.h>
#include <glib.h>

GHashTable *hash;

int fib(int num);

int main()
{
    int i, f;
    // initialize hash table
    hash = g_hash_table_new(g_int_hash, g_int_equal);

    printf("\nFibonacci Wert von : ");
    scanf("%d", &f);
    for (i = 0; i < f; i++)
    {
        printf("%d ", fib(i));
    }

    printf("\n=== MAP ===\n");

    printf("\nThere are %d keys in the hash\n", g_hash_table_size(hash));
    g_hash_table_destroy(hash);

    return 0;
}

int fib(int num)
{
    gint *key = g_new(gint, 1);
    gint *value = g_new(gint, 1);
    gint *value1 = g_new(gint, 1);
    gint *value2 = g_new(gint, 1);

    *key = num;

    /*if (*(int *)g_hash_table_lookup(hash, key) != NULL)
        return (*(int *)g_hash_table_lookup(hash, key));
    if (num <= 2)
        return num;
    */
    *value1 = *(int *)g_hash_table_lookup(hash, key - 1);
    *value2 = *(int *)g_hash_table_lookup(hash, key - 2);
    *value = *value1 + *value2;

    g_hash_table_insert(hash, key, value);
    // kv_insert(num, fib(num - 1) + fib(num - 2));
    return (*(int *)g_hash_table_lookup(hash, key));
    // return kv_get(num);
}
