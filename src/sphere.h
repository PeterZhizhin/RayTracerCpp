//
// Created by Петр on 04.07.2020.
//

#ifndef RAYTRACERPROJECT_SPHERE_H
#define RAYTRACERPROJECT_SPHERE_H

#include <optional>
#include <memory>
#include "absorbing.h"
#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

namespace ray_tracer::geometry {
    class Sphere : public Hittable {
    public:
        using Hittable::HitRecord;

        Sphere(const vector::Point3 &center, float radius,
               std::unique_ptr<material::Material> material = std::make_unique<material::AbsorbingMaterial>())
                : center_(center),
                  radius_(radius), material_(std::move(material)) {}

        virtual ~Sphere() override = default;

        [[nodiscard]] std::optional<HitRecord>
        hit(const ray::Ray &ray, float t_min, float t_max) const noexcept override;

    private:
        vector::Point3 center_;
        float radius_;
        std::unique_ptr<material::Material> material_;
    };
}


#endif //RAYTRACERPROJECT_SPHERE_H
