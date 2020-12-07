#include "camera.h"

void camera_cast_ray(struct ray *ray, const struct camera *camera, double cam_x,
                     double cam_y)
{
    // translate relative position inside the image plane
    // into absolute position into the image plane.
    double x_coeff = cam_x * camera->width;
    double y_coeff = cam_y * camera->height;

    struct vec3 right = vec3_cross(&camera->forward, &camera->up);
    // right_offset = right * x_coeff
    struct vec3 right_offset = vec3_mul(&right, x_coeff);
    // up_offset = up * y_coeff
    struct vec3 up_offset = vec3_mul(&camera->up, y_coeff);
    // offset = right_offset + up_offset
    struct vec3 offset = vec3_add(&right_offset, &up_offset);
    // ray->source = center + offset
    ray->source = vec3_add(&camera->center, &offset);

    struct vec3 vantage_point_offset
        = vec3_mul(&camera->forward, -camera->focal_distance);
    struct vec3 vantage_point
        = vec3_add(&vantage_point_offset, &camera->center);
    ray->direction = vec3_sub(&ray->source, &vantage_point);
    vec3_normalize(&ray->direction);
}
