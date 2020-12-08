#pragma once

#include "camera.h"
#include "object.h"

#include "utils/pvect.h"

// this code creates a new type of vector using C's
// poor man template metaprogramming™
#define GVECT_NAME object_vect
#define GVECT_TYPE struct object *
#include "utils/pvect_wrap.h"
#undef GVECT_NAME
#undef GVECT_TYPE

/* The scene contains all the objects, lights, and cameras.
** for simplicity scene, this scene type only handles a single light and
** a single camera.
*/
struct scene
{
    // the list of objects in the scene
    struct object_vect objects;

    // a very hacky single light
    // TODO: handle multiple lights
    struct vec3 light_color;
    struct vec3 light_direction;
    double light_intensity;

    struct camera camera;
};

static inline void scene_init(struct scene *scene)
{
    object_vect_init(&scene->objects, 42);
}

void scene_destroy(struct scene *scene);
