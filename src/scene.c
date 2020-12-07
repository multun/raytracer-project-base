#include "scene.h"

void scene_destroy(struct scene *scene)
{
    for (size_t i = 0; i < object_vect_size(&scene->objects); i++)
    {
        struct object *obj = object_vect_get(&scene->objects, i);
        if (obj->free)
            obj->free(obj);
    }

    object_vect_destroy(&scene->objects);
}
