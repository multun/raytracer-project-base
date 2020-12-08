#pragma once

#include "object.h"
#include "utils/alloc.h"
#include "vec3.h"

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

void sphere_free(struct object *obj);

static inline struct sphere *sphere_create(struct vec3 center, double radius,
                                           struct material *mat)
{
    struct sphere *sphere = zalloc(sizeof(*sphere));
    object_init(&sphere->base, object_sphere_ray_intersect, sphere_free);
    sphere->center = center;
    sphere->radius = radius;
    sphere->material = material_get(mat);
    return sphere;
}
