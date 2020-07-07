//
// Created by Петр on 07.07.2020.
//

#ifndef RAYTRACERPROJECT_CAMERA_H
#define RAYTRACERPROJECT_CAMERA_H

#include <cstdint>

#include "vec3.h"
#include "ray.h"

namespace ray_tracer::camera {
    class Camera {
    public:
        Camera(uint32_t width, uint32_t height, float focal_length = 1.0f, float viewport_height = 2.0f)
                : origin_{},
                  horizontal_{
                          viewport_height * static_cast<float>(width) / static_cast<float>(height),
                          0.0f, 0.0f},
                  vertical_{0.0f, viewport_height, 0.0f},
                  lower_left_corner_{
                          origin_ -
                          horizontal_ / 2.0f - vertical_ / 2.0f -
                          vector::Vec3{0.0f, 0.0f, focal_length}} {}

        ray::Ray get_ray(float u, float v);

    private:
        vector::Point3 origin_;
        vector::Point3 horizontal_;
        vector::Point3 vertical_;
        vector::Point3 lower_left_corner_;
    };
}


#endif //RAYTRACERPROJECT_CAMERA_H
