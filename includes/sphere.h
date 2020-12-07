#pragma once

#include "vec3.h"
#include "object.h"


struct sphere
{
    struct vec3 center;
    double radius;
};


double sphere_ray_intersect(struct intersection *intersection,
                            const struct ray *ray,
                            const struct sphere *sphere);
