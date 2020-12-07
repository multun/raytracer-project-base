#pragma once

#include "vec3.h"
#include "ray.h"


/*
** The location and normal of an intersection.
*/
struct intersection
{
    struct vec3 point;
    struct vec3 normal;
};

/* The scene type needs to be forward declared, as the scene
** depends on objects, and objects reference object intersections
** which reference materials which reference the scene type.
*/
struct scene;

/* A generic material type.
** As how materials are shaded entirely depends on the shader type,
** all materials instances contain a pointer to a function doing just that.
*/
struct material
{
    /*
    ** A pointer to the shading function.
    */
    struct vec3 (*shade)(const struct material *material,
                         const struct intersection *inter,
                         const struct scene *scene,
                         const struct ray *ray);
};

/*
** The result of an intersection with an object.
*/
struct object_intersection
{
    struct intersection location;
    struct material *material;
};

/*
** The common interface for objects.
** Those only need an intersection function and a descructor.
** If more function pointers are added, they should probably be moved to constant memory.
*/
struct object
{
    double (*intersect)(struct object_intersection *inter,
                        const struct object *obj,
                        const struct ray *ray);
    void (*free)(struct object *object);
};
