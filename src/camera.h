//
// Created by Петр on 07.07.2020.
//

#ifndef RAYTRACERPROJECT_CAMERA_H
#define RAYTRACERPROJECT_CAMERA_H

#include <cstdint>
#include <cmath>

#include "vec3.h"
#include "ray.h"

namespace ray_tracer::camera {
    class Camera {
    public:
        Camera(uint32_t width, uint32_t height, float focal_length = 1.0f,
               float vertical_field_of_view = static_cast<float>(M_PI_2))
                : origin_{} {
            auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
            auto h = std::tan(vertical_field_of_view / 2.0f);
            auto viewport_height = 2 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            horizontal_ = {viewport_width, 0.0f, 0.0f};
            vertical_ = {0.0f, viewport_height, 0.0f};
            lower_left_corner_ = {
                    origin_ -
                    horizontal_ / 2.0f - vertical_ / 2.0f -
                    vector::Vec3{0.0f, 0.0f, focal_length}};
        }

        ray::Ray get_ray(float u, float v);

    private:
        vector::Point3 origin_;
        vector::Point3 horizontal_;
        vector::Point3 vertical_;
        vector::Point3 lower_left_corner_;
    };
}


#endif //RAYTRACERPROJECT_CAMERA_H
