#pragma once

#include "ray.h"
#include "utils/refcnt.h"
#include "vec3.h"

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

struct material;

/* A pointer to a shading function.
 */
typedef struct vec3 (*material_shader_f)(const struct material *material,
                                         const struct intersection *inter,
                                         const struct scene *scene,
                                         const struct ray *ray);

/* A generic material type.
** As how materials are shaded entirely depends on the shader type,
** all materials instances contain a pointer to a function doing just that.
*/
struct material
{
    // a reference counter. should be the first field!
    struct refcnt refcnt;

    // a shading function
    material_shader_f shade;
};

typedef void (*material_free_f)(struct material *mat);

static inline void material_init(struct material *mat, material_free_f mat_free,
                                 material_shader_f mat_shader)
{
    // this cast is safe as refcnt is the first field of material
    ref_init(&mat->refcnt, (refcnt_free_f)mat_free);
    mat->shade = mat_shader;
}

#define MATERIAL_STATIC_INIT(Shader)                                           \
    {                                                                          \
        .refcnt = REFCNT_STATIC_INIT, .shade = (Shader)                        \
    }

// increases the material reference counter
static inline struct material *material_get(struct material *mat)
{
    ref_get(&mat->refcnt);
    return mat;
}

// decreases the material reference counter
static inline void material_put(struct material *mat)
{
    ref_put(&mat->refcnt);
}

/*
** The result of an intersection with an object.
*/
struct object_intersection
{
    struct intersection location;
    struct material *material;
};

struct object;

typedef void (*object_free_f)(struct object *obj);

typedef double (*object_intersect_f)(struct object_intersection *inter,
                                     const struct object *obj,
                                     const struct ray *ray);

/*
** The common interface for objects.
** Those only need an intersection function and a descructor.
** If more function pointers are added, they should probably be moved to
*constant memory.
*/
struct object
{
    object_intersect_f intersect;
    object_free_f free;
};

static inline void object_init(struct object *obj, object_intersect_f intersect,
                               object_free_f free)
{
    obj->intersect = intersect;
    obj->free = free;
}
