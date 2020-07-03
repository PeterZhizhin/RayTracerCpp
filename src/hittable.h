#ifndef RAYTRACERPROJECT_HITTABLE_H
#define RAYTRACERPROJECT_HITTABLE_H

#include <optional>
#include "vec3.h"
#include "ray.h"

namespace ray_tracer::hittable {
    class Hittable {
    public:
        struct HitRecord {
            vector::Point3 hit_point;
            vector::Vec3 normal_at_hit;
            float ray_t = 0.0f;
        };

        virtual ~Hittable() {}

        [[nodiscard]] virtual std::optional<HitRecord>
        hit(const ray::Ray &ray, float t_min, float t_max) const noexcept = 0;
    };
}

#endif //RAYTRACERPROJECT_HITTABLE_H
