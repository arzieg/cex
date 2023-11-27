#include <stdio.h>
#include <stdint.h>

int main()
{
    long int num = 1;
    intptr_t *pi = &num;
    uintptr_t *pu = (uintptr_t *)&num;

    return 0;
}