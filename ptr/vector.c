#include <stdio.h>

int main()
{

    int vector[] = {28, 41, 7};
    int *pi = vector;
    printf("\n%d", *pi);
    pi++;
    printf("\n%d", *pi);
    pi++;
    printf("\n%d\n", *pi);

    return 0;
}