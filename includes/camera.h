#pragma once

#include "ray.h"
#include "vec3.h"
#include <math.h>

struct camera
{
    struct vec3 center;
    struct vec3 forward;
    struct vec3 up;

    double width;
    double height;

    double focal_distance;
};

static inline double focal_distance_from_fov(double width, double fov_deg)
{
    // convert from degrees to radians
    double fov_rad = fov_deg * M_PI / (360 / 2);
    return (width / 2) / tan(fov_rad / 2);
}

/*
** The camera is a physical object in its own right, positioned in space just
** like any other. When casting a ray, the raytracer must express the
** coordinates of the starting point of the ray, relative to the image plane
** defined by the camera.
**
** One way to do it is to define the bottom left corner of the image plane to be
** at (-0.5, 0.5), its center to be at (0, 0), and its top right corner to be at
** (0.5, 0.5).
**
** This way, the camera doesn't have to know about the dimensions of the output
** image: it just traces rays where asked to.
**
**  (x=-0.5, y=0.5)                (x=0.5, y=0.5)
**        +------------------------------+
**        |                              |
**        |              ^ y             |
**        |              |               |
**        |              +---> x         |
**        |            center            |
**        |                              |
**        |                              |
**        +------------------------------+
** (x=-0.5, y=-0.5)                (x=0.5, y=-0.5)
*/
void camera_cast_ray(struct ray *ray, const struct camera *camera, double cam_x,
                     double cam_y);
