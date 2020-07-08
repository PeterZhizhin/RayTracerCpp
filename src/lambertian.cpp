#include "lambertian.h"

using ray_tracer::ray::Ray;
using ray_tracer::geometry::Hittable;

namespace ray_tracer::material {
    [[nodiscard]] std::optional<Material::ScatterInfo>
    LambertianMaterial::scatter(const Ray &in_ray,
                                const Hittable::HitRecord &hit_record) const noexcept {
        auto random_on_unit_sphere = random_.on_unit_sphere_uniform();
        auto target_direction = hit_record.normal_at_hit + random_on_unit_sphere;
        // color_multiplier = <target / target.length(); normal> = <target; normal> / target.length() =
        // = <normal + random; normal> / target.length() = (<normal; normal> + <random; normal>) / target.length() =
        // = [normal.length() == 1, target.length() == sqrt(<target; target>)] =
        // = (1 + <random; normal>) / sqrt(<normal + random; normal + random>) =
        // = (1 + <random; normal>) / sqrt(<normal; normal> + 2<normal; random> + <random; random>) =
        // = [random.length() == 1] = (1 + <random; normal>) / sqrt(1 + 2<normal; random> + 1) =
        // = (1 + <random; normal>) / sqrt(2) / sqrt(1 + <random; normal>) = sqrt((1 + <random; normal>) / 2)
        auto color_multiplier = std::sqrt((1 + random_on_unit_sphere.dot_product(hit_record.normal_at_hit)) / 2.0f);
        auto scattered_ray = Ray{hit_record.hit_point, target_direction};
        auto attenuated_color = color_multiplier * albedo_;
        return Material::ScatterInfo{scattered_ray, attenuated_color};
    }
}
