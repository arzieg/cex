#ifndef __OBJECTPOOL_H__
#define __OBJECTPOOL_H__

#include <stdbool.h>

typedef struct
{
  int x, y, z;
} Vector3;

typedef struct
{
  bool allocated;
  Vector3 obj;
} PoolObject;

#define NUM_OBJECTS 10

Vector3 *BorrowVector3 (void);
void ReturnVector3 (Vector3 *v);

#endif