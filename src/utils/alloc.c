#include "utils/alloc.h"
#include <err.h>
#include <stdlib.h>
#include <string.h>

__attribute__((malloc)) void *xalloc(size_t size)
{
    void *res = malloc(size);
    if (res == NULL && size > 0)
        abort();
    return res;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret = realloc(ptr, size);
    if (size && !ret)
        err(1, "realloc failed");
    return ret;
}

__attribute__((malloc)) void *xcalloc(size_t nmemb, size_t size)
{
    void *ret = calloc(nmemb, size);
    if (size && nmemb && !ret)
        err(1, "calloc failed");
    return ret;
}

__attribute__((malloc)) void *zalloc(size_t size)
{
    void *res = malloc(size);
    memset(res, 0, size);
    return res;
}
