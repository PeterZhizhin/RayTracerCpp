#ifndef RAYTRACERPROJECT_ABSORBING_H
#define RAYTRACERPROJECT_ABSORBING_H

#include <optional>
#include "material.h"

namespace ray_tracer::material {
    class AbsorbingMaterial : public Material {
    public:
        [[nodiscard]] std::optional<ScatterInfo>
        scatter(const ray::Ray &in_ray, const geometry::Hittable::HitRecord &hit_record) const noexcept override;

        ~AbsorbingMaterial() override = default;
    };
}


#endif //RAYTRACERPROJECT_ABSORBING_H
