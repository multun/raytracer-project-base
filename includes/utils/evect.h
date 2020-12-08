#pragma once

#define GVECT_NAME evect
#define GVECT_TYPE char

#include "utils/gvect.h"

#undef GVECT_NAME
#undef GVECT_TYPE

/*
** Push a string to the end of the vector
*/
void evect_push_string(struct evect *vect, const char *str);

/* add a temporary nul byte (not accounted in size) */
static inline void evect_finalize(struct evect *vect)
{
    evect_push(vect, '\0');
    vect->size--;
}
