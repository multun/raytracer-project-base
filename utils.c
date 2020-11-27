#include "utils.h"
#include <stdlib.h>

__attribute__((malloc)) void *xalloc(size_t size)
{
  void *res = malloc(size);
  if (res == NULL && size > 0)
    abort();
  return res;
}
