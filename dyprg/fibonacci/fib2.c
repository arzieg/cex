// Fibonacci Brute Force
// Beispiel

// Time Complexity O(2^n)
// Space Complexity O(n)

#include <stdio.h>
#include "kvs.h"

int fib(int number, KVSstore *kvs);

int main()
{
    int i, f;
    KVSstore *kvs;
    kvs = kvs_create();

    printf("\nFibonacci Wert von : ");
    scanf("%d", &f);
    for (i = 0; i < f; i++)
    {
        printf("%d ", fib(i, kvs));
    }

    return 0;
}

int fib(int num, KVSstore *fibkvs)
{

    if (num <= 2)
        return num;
    kvs_insert(fibkvs, (int *)num, (int *)num);
    // kvs_insert(fibkvs, &num, (int *)fib(num - 1, fibkvs) + (int *)fib(num - 2, fibkvs));
    //  return kvs_get(fibkvs, &num);
    return 0;
}
