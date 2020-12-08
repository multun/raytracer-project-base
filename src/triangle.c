#include "triangle.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define INTER_EPSILON 0.0000001

double object_triangle_ray_intersect(struct object_intersection *inter,
                                     const struct object *obj,
                                     const struct ray *ray)
{
    struct triangle *trian = (struct triangle *)obj;

    /*        0
    **        o
    **       / \
    **   a  /   \  c
    **     /     \
    **    /       \
    ** 1 o---------o 2
    **        b
    **
    ** The facing side is the one where points appear counter-clockwise.
    ** It's a somewhat arbitrary choice. I picked this way because of OpenGL.
    */

    const struct vec3 *v0 = &trian->points[0];
    const struct vec3 *v1 = &trian->points[1];
    const struct vec3 *v2 = &trian->points[2];

    struct vec3 a = vec3_sub(v1, v0);
    struct vec3 b = vec3_sub(v2, v1);
    struct vec3 c = vec3_sub(v0, v2);

    // compute the face's normal vector
    struct vec3 n = vec3_cross(&a, &b);

    // if the normal and the ray direction have the same sign, then the triangle
    // is facing the wrong way
    if (vec3_dot(&ray->direction, &n) >= 0)
        return INFINITY;

    // compute the distance from the plane to (0, 0, 0)
    // (aka the fourth plane equation component)
    double D = -vec3_dot(&n, v0);
    double t
        = -(vec3_dot(&n, &ray->source) + D) / vec3_dot(&n, &ray->direction);
    if (t < 0)
        return INFINITY;

    // P = O + t * dir
    struct vec3 P_off = vec3_mul(&ray->direction, t);
    struct vec3 P = vec3_add(&ray->source, &P_off);

    // check on which side of a, b, and c P is

    struct vec3 v0_to_p = vec3_sub(&P, v0);
    struct vec3 v0_cross = vec3_cross(&a, &v0_to_p);
    if (vec3_dot(&v0_cross, &n) < -INTER_EPSILON)
        return INFINITY;

    struct vec3 v1_to_p = vec3_sub(&P, v1);
    struct vec3 v1_cross = vec3_cross(&b, &v1_to_p);
    if (vec3_dot(&v1_cross, &n) < -INTER_EPSILON)
        return INFINITY;

    struct vec3 v2_to_p = vec3_sub(&P, v2);
    struct vec3 v2_cross = vec3_cross(&c, &v2_to_p);
    if (vec3_dot(&v2_cross, &n) < -INTER_EPSILON)
        return INFINITY;

    // if P is on the right side of the triangle's edges,
    // it is inside the triangle, and there is an intersection
    inter->material = trian->material;
    vec3_normalize(&n);
    inter->location.normal = n;
    inter->location.point = P;
    return t;
}

void triangle_free(struct object *obj)
{
    struct triangle *trian = (struct triangle *)obj;
    material_put(trian->material);
    free(trian);
}
