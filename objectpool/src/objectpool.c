#include "objectpool.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// declare my object pool

PoolObject object_pool[NUM_OBJECTS] = { 0 };

Vector3 *
BorrowVector3 (void)
{
  for (int i = 0; i < NUM_OBJECTS; i++)
    {
      if (object_pool[i].allocated == false)
        {
          object_pool[i].allocated = true;
          return &(object_pool[i].obj);
        }
    }
  return NULL;
}
/*
void
ReturnVector3 (Vector3 *v)
{
  for (int i = 0; i < NUM_OBJECTS; i++)
    {
      if (&(object_pool[i].obj) == v)
        {
          assert (object_pool[i].allocated);
          object_pool[i].allocated = false;
          return;
        }
    }
  assert (false); // this is a bug, look into it
}
*/

void
ReturnVector3 (Vector3 *v)
{
  long unsigned int i
      = ((uintptr_t)v - (uintptr_t)object_pool) / sizeof (PoolObject);
  assert (&(object_pool[i].obj) == v);
  assert (object_pool[i].allocated);
  object_pool[i].allocated = false;
}

int
main (void)
{
  for (int i = 0; i < 15; i++)
    {
      Vector3 *v1 = BorrowVector3 ();
      Vector3 *v2 = BorrowVector3 ();
      printf ("got vector  %d @ address %p %p\n", i, (void *)v1, (void *)v2);
      ReturnVector3 (v1);
      ReturnVector3 (v2);
    }
}
