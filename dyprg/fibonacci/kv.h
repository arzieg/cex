#ifndef __KV_H__
#define __KV_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_SIZE 1000 // Maximum number of elements in the map

extern int size;
extern int keys[MAX_SIZE];
extern uint64_t values[MAX_SIZE];

int kv_getIndex(int key);
void kv_insert(int key, uint64_t value);
int kv_get(int key);
void kv_printMap();

#endif