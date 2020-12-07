#pragma once

#include "vec3.h"
#include "object.h"

#include <stddef.h>

struct sphere
{
    struct object base;

    struct vec3 center;
    double radius;
    struct material *material;
};


double object_sphere_ray_intersect(struct object_intersection *inter,
                                   const struct object *obj,
                                   const struct ray *ray);

static inline void sphere_init(struct sphere *sphere)
{
    sphere->base.intersect = object_sphere_ray_intersect;
    // TODO: refcount materials and decrease the refcount on deletion
    sphere->base.free = NULL;
}
