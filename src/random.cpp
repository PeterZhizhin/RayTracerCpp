#include "random.h"

#include <cmath>
#include <random>
#include "vec3.h"

namespace ray_tracer::random {
    [[nodiscard]] float Random::uniform(float min, float max) noexcept {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator_);
    }

    [[nodiscard]] float Random::standard_uniform() noexcept {
        return uniform(0.0f, 1.0f);
    }

    [[nodiscard]] bool Random::with_prob(float prob) noexcept {
        return standard_uniform() < prob;
    }

    [[nodiscard]] vector::Vec3 Random::in_unit_sphere_uniform() noexcept {
        auto cube_proposal = [this]() -> vector::Vec3 {
            auto uniform_m11 = [this]() {
                return uniform(-1.0f, 1.0f);
            };
            return {uniform_m11(), uniform_m11(), uniform_m11()};
        };
        while (true) {
            auto proposed = cube_proposal();
            if (proposed.length2() <= 1.0f) {
                return proposed;
            }
        }
    }

    [[nodiscard]] vector::Vec3 Random::on_unit_sphere_uniform() noexcept {
        // #include <numbers> is not supported in GCC 9 yet :C
        auto angle = uniform(0.0f, 2 * M_PI);
        auto z_coord = uniform(-1.0f, 1.0f);
        auto z_radius = std::sqrt(1.0f - z_coord * z_coord);
        return {z_radius * std::cos(angle), z_radius * std::sin(angle), z_coord};
    }

    [[nodiscard]] vector::Vec3 Random::in_unit_disk_uniform() noexcept {
        auto square_proposal = [this]() -> vector::Vec3 {
            return {standard_uniform(), standard_uniform(), 0.0f};
        };
        while (true) {
            auto proposal = square_proposal();
            if (proposal.length2() <= 1) {
                return proposal;
            }
        }
    }
}
