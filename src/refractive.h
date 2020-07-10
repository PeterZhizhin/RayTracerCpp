#ifndef RAYTRACERPROJECT_REFRACTIVE_H
#define RAYTRACERPROJECT_REFRACTIVE_H

#include <optional>

#include "hittable.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "vec3.h"

namespace ray_tracer::material {
    class RefractiveMaterial : public Material {
    public:
        explicit RefractiveMaterial(float refractive_index, random::Random &random) : refractive_index_(
                refractive_index), random_(random) {}


        [[nodiscard]] std::optional<Material::ScatterInfo>
        scatter(const ray::Ray &in_ray, const geometry::Hittable::HitRecord &hit_record) const noexcept override;

    private:
        float refractive_index_;
        random::Random &random_;
    };
}


#endif //RAYTRACERPROJECT_REFRACTIVE_H
