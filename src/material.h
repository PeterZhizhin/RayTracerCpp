#ifndef RAYTRACERPROJECT_MATERIAL_H
#define RAYTRACERPROJECT_MATERIAL_H

#include <optional>
#include <utility>
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

namespace ray_tracer::material {
    class Material {
    public:

        struct ScatterInfo {
            ray::Ray ray;
            vector::Color3 attenuation;
        };

        [[nodiscard]] virtual std::optional<ScatterInfo>
        scatter(const ray::Ray &in_ray, const geometry::Hittable::HitRecord &hit_record) const noexcept = 0;

        virtual ~Material() = default;
    };

}

#endif //RAYTRACERPROJECT_MATERIAL_H
