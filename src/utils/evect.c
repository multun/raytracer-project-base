#include "utils/evect.h"

#define GVECT_NAME evect
#define GVECT_TYPE char
#include "gvect.defs"

void evect_push_string(struct evect *vect, const char *str)
{
    for (; *str; str++)
        evect_push(vect, *str);
}
