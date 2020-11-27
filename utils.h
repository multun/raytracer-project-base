#pragma once

#include <stddef.h>

#define STATIC_ASSERT(Name, X)                                                 \
    struct __assert_##Name                                                     \
    {                                                                          \
        char Name : (1 - 2 * (!(X)));                                          \
    }

static inline size_t align_up(size_t x, size_t alignment)
{
    size_t res = x + alignment - 1;
    return res - res % alignment;
}

__attribute__((malloc)) void *xalloc(size_t size);
