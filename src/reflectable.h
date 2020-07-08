#ifndef RAYTRACERPROJECT_REFLECTABLE_H
#define RAYTRACERPROJECT_REFLECTABLE_H

#include "material.h"
#include "hittable.h"
#include "ray.h"

namespace ray_tracer::material {
    class ReflectableMaterial : public Material {
    public:
        explicit ReflectableMaterial(const vector::Color3 &albedo) : albedo_(albedo) {}

        [[nodiscard]] std::optional<Material::ScatterInfo>
        scatter(const ray::Ray &in_ray, const geometry::Hittable::HitRecord &hit_record) const noexcept override;

        ~ReflectableMaterial() override = default;

    private:
        vector::Color3 albedo_;
    };
}


#endif //RAYTRACERPROJECT_REFLECTABLE_H
