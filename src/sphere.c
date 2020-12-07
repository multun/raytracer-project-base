#include "sphere.h"

#include <stdlib.h>

static double sphere_ray_intersect(struct intersection *intersection,
                                   const struct sphere *sphere,
                                   const struct ray *ray)
{
    struct vec3 hypothenuse = vec3_sub(&sphere->center, &ray->source);
    double hyp_len = vec3_length(&hypothenuse);
    double projection = vec3_dot(&hypothenuse, &ray->direction);
    if (projection < 0)
        return INFINITY;

    double d = sqrt(hyp_len * hyp_len - projection * projection);
    if (d > sphere->radius)
        return INFINITY;

    double radius = sphere->radius;
    double m = sqrt(radius * radius - d * d);
    double t0 = projection - m;
    double t1 = projection + m;

    double t = t0;
    if (t < 0.)
        t = t1;

    // intersection point = ray->source + ray->direction * t
    struct vec3 point_offset = vec3_mul(&ray->direction, t);
    intersection->point = vec3_add(&ray->source, &point_offset);
    intersection->normal = vec3_sub(&intersection->point, &sphere->center);
    vec3_normalize(&intersection->normal);
    // compute intersection coord / normal
    return t;
}

double object_sphere_ray_intersect(struct object_intersection *inter,
                                   const struct object *obj,
                                   const struct ray *ray)
{
    const struct sphere *sphere = (const struct sphere *)obj;
    double inter_dis = sphere_ray_intersect(&inter->location, sphere, ray);
    if (isinf(inter_dis))
        return inter_dis;

    inter->material = sphere->material;
    return inter_dis;
}

void sphere_free(struct object *obj)
{
    struct sphere *sphere = (struct sphere *)obj;
    material_put(sphere->material);
    free(sphere);
}
