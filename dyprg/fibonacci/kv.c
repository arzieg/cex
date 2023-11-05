#include "kvs.h"
#include <stdio.h>

int main()
{
    KVSstore *kvs;
    int key = 1;
    int value = 5;
    int *pkey = &key;
    int *pvalue = &value;
    kvs_insert(kvs, (int *)key, (int *)value);
    return 0;
}
