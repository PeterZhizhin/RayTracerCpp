#include "camera.h"
#include "vec3.h"
#include "ray.h"

namespace ray_tracer::camera {
    [[nodiscard]] ray::Ray Camera::get_ray(float u, float v) const noexcept {
        auto disk_vector = lens_radius_ * random_.in_unit_disk_uniform();
        auto offset = disk_vector.x() * u_ + disk_vector.y() * v_;
        return ray::Ray{origin_ - offset,
                        lower_left_corner_ + u * horizontal_ + v * vertical_ + offset};
    }
}
