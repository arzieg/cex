// Fibonacci Brute Force
// Beispiel

// Time Complexity O(2^n)
// Space Complexity O(n)

#include <stdio.h>

int fib(int number);

int main()
{
    int i, f;
    printf("\nFibonacci Wert von : ");
    scanf("%d", &f);
    for (i = 0; i < f; i++)
    {
        printf("%d ", fib(i));
    }

    return 0;
}

int fib(int num)
{
    if (num <= 2)
        return num;
    return (fib(num - 1) + fib(num - 2));
}
