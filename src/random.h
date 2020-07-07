#ifndef RAYTRACERPROJECT_RANDOM_H
#define RAYTRACERPROJECT_RANDOM_H

#include <random>

namespace ray_tracer::random {
    class Random {
    public:
        float standard_uniform();
    private:
        std::mt19937 generator_;
    };
}


#endif //RAYTRACERPROJECT_RANDOM_H
