#include "refractive.h"

#include <cmath>
#include <optional>

#include "hittable.h"
#include "ray.h"
#include "reflectable.h"
#include "vec3.h"

using ray_tracer::vector::Vec3;
using ray_tracer::vector::Color3;
using ray_tracer::ray::Ray;

namespace ray_tracer::material {

    namespace {
        Vec3 refract(const Vec3 &unit_direction, const Vec3 &normal, float refractive_index_fraction,
                     float normal_direction_angle_cos) {
            auto orthogonal_part = refractive_index_fraction * (unit_direction + normal_direction_angle_cos * normal);
            auto parallel_part = -std::sqrt(1 - orthogonal_part.length2()) * normal;
            return orthogonal_part + parallel_part;
        }

        float get_reflection_prob(float normal_direction_angle_cos, float refractive_index_fraction) {
            auto r0 = (1 - refractive_index_fraction) / (1 + refractive_index_fraction);
            r0 *= r0;
            auto one_minus_cos = 1 - normal_direction_angle_cos;
            auto second_factor = std::pow(one_minus_cos, 5);
            auto result = r0 + (1 - r0) * second_factor;
            return result;
        }
    }

    [[nodiscard]] std::optional<Material::ScatterInfo>
    RefractiveMaterial::scatter(const ray::Ray &in_ray,
                                const geometry::Hittable::HitRecord &hit_record) const noexcept {
        float refractive_index_fraction = 1.0f / refractive_index_;
        if (!hit_record.front_face) {
            refractive_index_fraction = refractive_index_;
        }
        const auto normalized_direction = in_ray.direction().unit();
        const auto normal_direction_angle_cos = -normalized_direction.dot_product(hit_record.normal_at_hit);
        const auto normal_direction_angle_sin2 = 1 - normal_direction_angle_cos * normal_direction_angle_cos;

        const auto reflection_prob = get_reflection_prob(normal_direction_angle_cos, refractive_index_fraction);
        if (refractive_index_fraction * refractive_index_fraction * normal_direction_angle_sin2 > 1.0f ||
            random_.with_prob(reflection_prob)) {
            // Two options
            // 1. must reflect here, because sin(theta') > 1.0.
            // 2. decided to reflect here with the approximation defined above.
            return ReflectableMaterial{Color3{1.0f, 1.0f, 1.0f}}.scatter(in_ray, hit_record);
        }

        const auto scattered_direction = refract(normalized_direction, hit_record.normal_at_hit,
                                                 refractive_index_fraction,
                                                 normal_direction_angle_cos);
        const Ray scattered_ray{hit_record.hit_point, scattered_direction};
        return Material::ScatterInfo{.ray = scattered_ray, .attenuation = Color3{1.0f, 1.0f, 1.0f}};
    }
}
