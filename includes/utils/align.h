#pragma once

#include <stddef.h>

static inline size_t align_up(size_t x, size_t alignment)
{
    size_t res = x + alignment - 1;
    return res - res % alignment;
}
