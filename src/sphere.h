//
// Created by Петр on 04.07.2020.
//

#ifndef RAYTRACERPROJECT_SPHERE_H
#define RAYTRACERPROJECT_SPHERE_H

#include <optional>
#include "hittable.h"

namespace ray_tracer::geometry {
    class Sphere : public hittable::Hittable {
    public:
        using hittable::Hittable::HitRecord;

        Sphere(const vector::Point3 &center, float radius) : center_(center), radius_(radius) {}

        virtual ~Sphere() override {}

        [[nodiscard]] virtual std::optional<HitRecord>
        hit(const ray::Ray &ray, float t_min, float t_max) const noexcept override;

    private:
        vector::Point3 center_;
        float radius_;
    };
}


#endif //RAYTRACERPROJECT_SPHERE_H
