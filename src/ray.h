//
// Created by Петр on 03.07.2020.
//

#ifndef RAYTRACERPROJECT_RAY_H
#define RAYTRACERPROJECT_RAY_H

#include "vec3.h"

namespace ray_tracer::ray {

    class Ray {
    public:
        Ray() = default;

        Ray(const vector::Point3 &origin, const vector::Vec3 &direction) : origin_(origin), direction_(direction) {}

        [[nodiscard]] constexpr const vector::Point3 &origin() const noexcept {
            return origin_;
        }

        [[nodiscard]] constexpr const vector::Vec3 &direction() const noexcept {
            return direction_;
        }

        [[nodiscard]] constexpr vector::Point3 at(float t) const noexcept {
            return origin_ + t * direction_;
        }

        [[nodiscard]] constexpr bool operator==(const Ray& ray) const noexcept {
            return (origin_ == ray.origin_) && (direction_ == ray.direction_);
        }

        [[nodiscard]] constexpr bool operator!=(const Ray& ray) const noexcept {
            return !(*this == ray);
        }

    private:
        vector::Point3 origin_;
        vector::Vec3 direction_;
    };

}


#endif //RAYTRACERPROJECT_RAY_H
