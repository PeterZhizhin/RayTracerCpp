//
// Created by Петр on 04.07.2020.
//

#include "sphere.h"

#include <optional>
#include "vec3.h"
#include "ray.h"

namespace ray_tracer::geometry {

    [[nodiscard]] std::optional<Sphere::HitRecord>
    Sphere::hit(const ray::Ray &ray, float t_min, float t_max) const noexcept {
        const auto origin_minus_center = ray.origin() - center_;
        const auto quadratic_a = ray.direction().length2();
        const auto quadratic_half_b = origin_minus_center.dot_product(ray.direction());
        const auto quadratic_c = origin_minus_center.length2() - radius_ * radius_;
        const auto discriminant = quadratic_half_b * quadratic_half_b - quadratic_a * quadratic_c;
        if (discriminant >= 0) {
            const auto sqrt_discriminant = std::sqrt(discriminant);
            for (const auto sign : {-1, +1}) {
                const auto result_t = (-quadratic_half_b + sign * sqrt_discriminant) / quadratic_a;
                if (t_min <= result_t && result_t <= t_max) {
                    auto hit_point = ray.at(result_t);
                    auto normal_at_hit = (hit_point - center_).unit();
                    return Sphere::HitRecord{.hit_point = hit_point, .normal_at_hit = normal_at_hit, .ray_t = result_t};
                }
            }
        }
        return std::nullopt;
    }
}
