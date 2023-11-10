// Fibonacci Brute Force
// Beispiel

// Time Complexity O(2^n)
// Space Complexity O(n)

#include <stdio.h>
#include <stdint.h>
#include <glib.h>

GHashTable *hash;

int fib(guint64 num);

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

int fib(guint64 num)
{

    if (*(int *)g_hash_table_lookup(hash, num) != NULL)
        return (*(int *)g_hash_table_lookup(hash, num));
    if (num <= 2)
        return num;
    g_hash_table_insert(hash, num, fib(num - 1) + fib(num - 2));
    // kv_insert(num, fib(num - 1) + fib(num - 2));
    //  kvs_insert(fibkvs, &num, (int *)fib(num - 1, fibkvs) + (int *)fib(num - 2, fibkvs));
    return (*(int *)g_hash_table_lookup(hash, num));
    // return kv_get(num);
}
