/* allocate memory */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int *pi = malloc(5 * sizeof(int));
    memset(pi, 0, 5 * sizeof(int));

    // realloc
    char *string1;
    char *string2;
    string1 = (char *)malloc(16);
    strcpy(string1, "0123456789AB");
    string2 = realloc(string1, 64);
    printf("string1 value: %p [%s]\n", string1, string1);
    printf("string2 value: %p [%s]\n", string2, string2);
}