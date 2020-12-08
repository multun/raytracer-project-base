#pragma once

#include "object.h"
#include "utils/alloc.h"
#include "vec3.h"

#include <stddef.h>

/*
** The facing side of the triangle is the one where the points appear
** in counter clockwise order.
*/
struct triangle
{
    struct object base;
    struct vec3 points[3];
    struct material *material;
};

double object_triangle_ray_intersect(struct object_intersection *inter,
                                     const struct object *obj,
                                     const struct ray *ray);

void triangle_free(struct object *obj);

static inline struct triangle *triangle_create(struct vec3 points[3],
                                               struct material *mat)
{
    struct triangle *trian = zalloc(sizeof(*trian));
    object_init(&trian->base, object_triangle_ray_intersect, triangle_free);
    trian->points[0] = points[0];
    trian->points[1] = points[1];
    trian->points[2] = points[2];
    trian->material = material_get(mat);
    return trian;
}
