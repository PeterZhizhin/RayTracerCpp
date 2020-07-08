#ifndef RAYTRACERPROJECT_LAMBERTIAN_MATERIAL_H
#define RAYTRACERPROJECT_LAMBERTIAN_MATERIAL_H

#include <optional>
#include "material.h"
#include "random.h"

namespace ray_tracer::material {
    class LambertianMaterial : public Material {
    public:
        LambertianMaterial(const vector::Color3 &albedo, random::Random &random) : albedo_(albedo), random_(random) {}

        [[nodiscard]] std::optional<Material::ScatterInfo>
        scatter(const ray::Ray &in_ray, const geometry::Hittable::HitRecord &hit_record) const noexcept override;

        ~LambertianMaterial() override = default;

    private:
        vector::Color3 albedo_;
        random::Random& random_;
    };
}


#endif //RAYTRACERPROJECT_LAMBERTIAN_MATERIAL_H
