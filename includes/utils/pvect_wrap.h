#include "gvect_common.h"
#include <stddef.h>

#ifndef PVECT_INCLUDED
#error missing pvect include before pvect_wrap include
#endif

#ifndef GVECT_NAME
#error undefined GVECT_NAME in generic vector
#endif

#ifndef GVECT_TYPE
#error undefined GVECT_TYPE in generic vector
#endif

struct GVECT_NAME
{
    struct pvect base;
};

static inline size_t GVECT_FNAME(size)(struct GVECT_NAME *vect)
{
    return pvect_size(&vect->base);
}

static inline GVECT_TYPE *GVECT_FNAME(data)(struct GVECT_NAME *vect)
{
    return (GVECT_TYPE *)pvect_data(&vect->base);
}

static inline GVECT_TYPE GVECT_FNAME(get)(struct GVECT_NAME *vect, size_t i)
{
    return pvect_get(&vect->base, i);
}

#ifndef GVECT_INIT_SIZE
static inline void GVECT_FNAME(init)(struct GVECT_NAME *vect, size_t capacity)
{
    pvect_init(&vect->base, capacity);
}
#else
static inline void GVECT_FNAME(init)(struct GVECT_NAME *vect)
{
    pvect_init(&vect->base, GVECT_INIT_SIZE);
}
#endif

static inline void GVECT_FNAME(destroy)(struct GVECT_NAME *vect)
{
#ifdef GVECT_DESTROY_ITEM
    for (size_t i = 0; i < GVECT_FNAME(size)(vect); i++)
        GVECT_DESTROY_ITEM(GVECT_FNAME(get)(vect, i));
#endif
    pvect_destroy(&vect->base);
}

static inline void GVECT_FNAME(push)(struct GVECT_NAME *vect, GVECT_TYPE item)
{
    pvect_push(&vect->base, item);
}

static inline GVECT_TYPE GVECT_FNAME(pop)(struct GVECT_NAME *vect)
{
    return pvect_pop(&vect->base);
}

static inline GVECT_TYPE *GVECT_FNAME(last)(struct GVECT_NAME *vect)
{
    return (GVECT_TYPE *)pvect_last(&vect->base);
}

static inline void GVECT_FNAME(reset)(struct GVECT_NAME *vect)
{
    pvect_reset(&vect->base);
}

static inline void GVECT_FNAME(cut)(struct GVECT_NAME *vect, size_t i)
{
    pvect_cut(&vect->base, i);
}
