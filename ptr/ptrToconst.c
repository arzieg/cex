#include <stdio.h>

int main()
{
    int num = 5;
    const int limit = 500;
    int *pi;
    const int *pci;

    pi = &num;
    pci = &limit;

    printf(" num - Address: %p value: %d\n", &num, num);
    printf("limit - Address: %p value: %d\n", &limit, limit);
    printf("pi - Address: %p value: %p\n", &pi, pi);
    printf(" pci - Address: %p value: %p\n", &pci, pci);

    // ein const pointer kann einen andere Referenz erhalten
    pci = &num;
    printf(" pci - Address: %p value: %p\n", &pci, pci);

    // ein const pointer kann aber keinen anderen Wert annehmen
    // ptrToconst.c:23:10: error: assignment of read-only location ‘*pci’
    // *pci = 200;

    return 0;
}