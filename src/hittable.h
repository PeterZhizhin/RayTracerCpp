#ifndef RAYTRACERPROJECT_HITTABLE_H
#define RAYTRACERPROJECT_HITTABLE_H

#include <optional>
#include "ray.h"
#include "vec3.h"

namespace ray_tracer::material { class Material; }

namespace ray_tracer::geometry {
    class Hittable {
    public:
        struct HitRecord {
            constexpr HitRecord(const vector::Point3 &hit, const ray::Ray &ray, float t,
                                const vector::Vec3 &outward_normal,
                                const material::Material *in_material = nullptr) noexcept
                    : hit_point(hit), normal_at_hit(outward_normal), ray_t(t),
                      front_face(ray.direction().dot_product(normal_at_hit) < 0),
                      material(in_material) {
                if (!front_face) {
                    normal_at_hit = -normal_at_hit;
                }
            }

            vector::Point3 hit_point;
            vector::Vec3 normal_at_hit;
            float ray_t;
            bool front_face;
            const material::Material* material;
        };

        virtual ~Hittable() = default;

        [[nodiscard]] virtual std::optional<HitRecord>
        hit(const ray::Ray &ray, float t_min, float t_max) const noexcept = 0;
    };
}

#endif //RAYTRACERPROJECT_HITTABLE_H
