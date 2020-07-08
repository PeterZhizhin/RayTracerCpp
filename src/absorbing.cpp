#include "absorbing.h"

#include <optional>

#include "ray.h"
#include "hittable.h"

namespace ray_tracer::material {
    [[nodiscard]] std::optional<Material::ScatterInfo>
    AbsorbingMaterial::scatter(const ray::Ray &in_ray, const geometry::Hittable::HitRecord &hit_record) const noexcept {
        return std::nullopt;
    }
}
