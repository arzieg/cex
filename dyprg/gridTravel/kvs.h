#ifndef __KVS_H__
#define __KVS_H__

#include <stdlib.h>

typedef struct
{
    const void *key;
    void *value;
} KVSpair;

typedef struct
{
    KVSpair *pairs;
    size_t length;
} KVSstore;

KVSstore *kvs_create(void);

void kvs_destroy(KVSstore *store);

void kvs_insert(KVSstore *store, const void *key, void *value);

void *kvs_get(KVSstore *store, const void *key);

#endif