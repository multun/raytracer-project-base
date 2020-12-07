#pragma once

#include "object.h"

#include "utils/pvect.h"

#define GVECT_NAME object_vect
#define GVECT_TYPE struct object *
#include "utils/pvect_wrap.h"
#undef GVECT_NAME
#undef GVECT_TYPE

struct scene
{
    struct object_vect objects;
};
