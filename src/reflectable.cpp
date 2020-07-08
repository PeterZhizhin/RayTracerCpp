#include "reflectable.h"

#include "hittable.h"
#include "ray.h"
#include "vec3.h"

using ray_tracer::geometry::Hittable;
using ray_tracer::ray::Ray;
using ray_tracer::vector::Vec3;

namespace ray_tracer::material {
    namespace {
        auto reflect(const Vec3& vector, const Vec3& normal) {
            return vector - 2 * vector.dot_product(normal) * normal;
        }
    }

    [[nodiscard]] std::optional<Material::ScatterInfo>
    ReflectableMaterial::scatter(const Ray &in_ray,
                                 const Hittable::HitRecord &hit_record) const noexcept {
        auto reflected = reflect(in_ray.direction().unit(), hit_record.normal_at_hit);
        return Material::ScatterInfo{.ray = Ray{hit_record.hit_point, reflected}, .attenuation = albedo_};
    }

}
