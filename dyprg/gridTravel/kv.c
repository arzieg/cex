#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MAX_SIZE 1000 // Maximum number of elements in the map

int size = 0;
int keys[MAX_SIZE];
uint64_t values[MAX_SIZE];

int kv_getIndex(int key)
{
    for (int i = 0; i < size; i++)
        if (keys[i] == key)
            return i;
    return -1; // key not found
}

void kv_insert(int key, uint64_t value)
{
    int index = kv_getIndex(key);
    if (index == -1)
    { // key not found
        keys[size] = key;
        values[size] = value;
        size++;

        printf("\nkeys[size]=%d   values[size]=%ld    size=%d", key, value, size);
    }
    else
    {
        values[index] = value;
    }
}

int kv_get(int key)
{
    int index = kv_getIndex(key);
    if (index == -1)
    { // Key not found
        return -1;
    }
    else
    { // Key found
        return values[index];
    }
}

// Function to print the map
void kv_printMap()
{
    for (int i = 0; i < size; i++)
    {
        printf("%d: %ld\n", keys[i], values[i]);
    }
}
