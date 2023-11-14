#include <stdio.h>
#include <string.h>
#include "kvs.h"

int main()
{
    char key[100][6];
    int value[100];
    char searchkey[6];
    char *p_searchkey;

    KVSstore *kv;

    kv = kvs_create();

    strcpy(key[0], "ABCDE");
    value[0] = 10;
    strcpy(key[1], "FGHIJ");
    value[1] = 5;

    kvs_insert(kv, key[0], &value[0]);
    kvs_insert(kv, key[1], &value[1]);

    strcpy(searchkey, "ABCDE");
    p_searchkey = searchkey;

    printf("\nValue of Key %s is %d", p_searchkey, *(int *)kvs_get(kv, p_searchkey));

    return 0;
}