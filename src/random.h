#ifndef RAYTRACERPROJECT_RANDOM_H
#define RAYTRACERPROJECT_RANDOM_H

#include <random>
#include "vec3.h"

namespace ray_tracer::random {
    class Random {
    public:
        [[nodiscard]] float uniform(float min, float max) noexcept;
        [[nodiscard]] float standard_uniform() noexcept;
        [[nodiscard]] bool with_prob(float prob) noexcept;
        [[nodiscard]] vector::Vec3 in_unit_sphere_uniform() noexcept;
        [[nodiscard]] vector::Vec3 on_unit_sphere_uniform() noexcept;
        [[nodiscard]] vector::Vec3 in_unit_disk_uniform() noexcept;
    private:
        std::mt19937 generator_;
    };
}


#endif //RAYTRACERPROJECT_RANDOM_H
