#include "camera.h"
#include "vec3.h"
#include "ray.h"

namespace ray_tracer::camera {
    [[nodiscard]] ray::Ray Camera::get_ray(float u, float v) const noexcept {
        return ray::Ray{origin_, lower_left_corner_ + u * horizontal_ + v * vertical_};
    }
}
