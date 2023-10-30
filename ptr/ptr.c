#include <stdio.h>

int var = 1;

int *ptr;

int *p_array;
int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

int main(void)
{
    int x = 0;

    ptr = &var;
    printf("\nDirect access, var = %d", var);
    printf("\nIndirect access, var = %d", *ptr);

    printf("\nThe address of var = %d", &var);
    printf("\nThe address of var = %d\n", ptr);

    p_array = array;
    printf("\nThe p_address of array = %p", p_array);
    printf("\nThe address of array = %p\n", array);

    for (x = 0; x < 10; x++)
        printf("%d ", array[x]);

    printf("\n %d", *p_array);

    return 0;
}