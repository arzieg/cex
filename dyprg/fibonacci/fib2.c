// Fibonacci Brute Force
// Beispiel

// Time Complexity O(2^n)
// Space Complexity O(n)

#include <stdio.h>
#include <stdint.h>
#include "kv.h"

int fib(uint64_t number);

int main()
{
    int i, f;

    printf("\nFibonacci Wert von : ");
    scanf("%d", &f);
    for (i = 0; i < f; i++)
    {
        printf("%d ", fib(i));
    }

    printf("\n=== MAP ===\n");
    kv_printMap();

    return 0;
}

int fib(uint64_t num)
{
    if (kv_get(num) != -1)
        return kv_get(num);
    if (num <= 2)
        return num;
    kv_insert(num, fib(num - 1) + fib(num - 2));
    // kvs_insert(fibkvs, &num, (int *)fib(num - 1, fibkvs) + (int *)fib(num - 2, fibkvs));
    return kv_get(num);
}
