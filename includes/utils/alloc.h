#pragma once

#include <stddef.h>

__attribute__((malloc)) void *xalloc(size_t size);

void *xrealloc(void *ptr, size_t size);

__attribute__((malloc)) void *xcalloc(size_t nmemb, size_t size);

__attribute__((malloc)) void *zalloc(size_t size);
