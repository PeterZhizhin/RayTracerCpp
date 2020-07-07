#include "random.h"

#include <random>

namespace ray_tracer::random {
    float Random::standard_uniform() {
        std::uniform_real_distribution<float> distribution;
        return distribution(generator_);
    }
}
