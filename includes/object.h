#pragma once

#include "vec3.h"
#include "ray.h"

struct intersection
{
    struct vec3 point;
    struct vec3 normal;
};

struct scene;

struct material
{
    /*
    ** A pointer to the shading function.
    */
    struct vec3 (*shade)(struct material *material,
                         struct intersection *inter,
                         struct scene *scene,
                         struct ray *ray);
};

struct object_intersection
{
    struct intersection location;
    struct material *material;
};

struct object
{
    double (*intersect)(struct object_intersection *inter,
                        struct object *obj,
                        struct ray *ray);
    void (*free)(struct object *object);
};
