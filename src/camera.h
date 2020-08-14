#ifndef RAYTRACERPROJECT_CAMERA_H
#define RAYTRACERPROJECT_CAMERA_H

#include <cstdint>
#include <cmath>

#include "random.h"
#include "ray.h"
#include "vec3.h"

namespace ray_tracer::camera {
    class Camera {
    public:
        Camera(uint32_t width, uint32_t height,
               random::Random &random,
               float vertical_field_of_view = static_cast<float>(M_PI_2),
               vector::Point3 lookfrom = vector::Point3{},
               vector::Point3 lookat = vector::Point3{0.0f, 0.0f, -1.0f},
               vector::Vec3 vector_up = vector::Vec3{0.0f, 1.0f, 0.0f},
               float aperture = 0.0f,
               float focus_dist = 1.0f)
                : random_(random), origin_{lookfrom}, lens_radius_(aperture / 2.0f) {
            auto aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
            auto h = std::tan(vertical_field_of_view / 2.0f);
            auto viewport_height = 2 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            auto w = (lookfrom - lookat).unit();
            auto u = vector_up.cross(w).unit();
            auto v = w.cross(u);

            horizontal_ = focus_dist * viewport_width * u;
            vertical_ = focus_dist * viewport_height * v;
            lower_left_corner_ = -horizontal_ / 2.0f - vertical_ / 2.0f - focus_dist * w;
            u_ = u;
            v_ = v;
        }

        [[nodiscard]] ray::Ray get_ray(float u, float v) const noexcept;

    private:
        random::Random &random_;
        vector::Point3 origin_;
        vector::Point3 horizontal_;
        vector::Point3 vertical_;
        vector::Point3 lower_left_corner_;
        vector::Vec3 u_;
        vector::Vec3 v_;
        float lens_radius_;
    };
}


#endif //RAYTRACERPROJECT_CAMERA_H
