#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <cmath>
#include "hittable.h"
#include "random.h"
#include "ray.h"
#include "refractive.h"
#include "vec3.h"

using ray_tracer::geometry::Hittable;
using ray_tracer::material::RefractiveMaterial;
using ray_tracer::random::Random;
using ray_tracer::ray::Ray;
using ray_tracer::vector::Color3;
using ray_tracer::vector::Point3;
using ray_tracer::vector::Vec3;

namespace {
    constexpr float deg2rad(float deg) {
        return deg * static_cast<float>(M_PI / 180.0);
    }

    // For 45 degree angle, it reflected at least once with prob = (1 - 1e-14)
    constexpr inline size_t number_of_runs = 300;
}

TEST_CASE("When material 2.0 ref index, hit at 45 degree angle, returns normalized vector", "[refractive]") {
    Random rand;
    RefractiveMaterial material(2.0, rand);
    Ray in_ray{Point3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, -1.0f, 0.0f}};
    Hittable::HitRecord record{Point3{1.0f, 0.0f, 0.0f}, in_ray, 1.0f, Vec3{0.0f, 1.0f, 0.0f}};
    // As it's randomly either refracts or reflects, we make
    for (size_t i = 0; i != number_of_runs; ++i) {
        auto scatter_info = material.scatter(in_ray, record);
        REQUIRE(scatter_info.has_value());

        auto scattered_ray_direction = scatter_info->ray.direction();
        REQUIRE(scattered_ray_direction.length2() == Approx(1.0f));
    }
}

TEST_CASE("When material 2.0 ref index, hit at 45 degree angle, refracts at asin(0.5 * sin(45deg))", "[refractive]") {
    Random rand;
    RefractiveMaterial material(2.0, rand);
    Ray in_ray{Point3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, -1.0f, 0.0f}};
    Hittable::HitRecord record{Point3{1.0f, 0.0f, 0.0f}, in_ray, 1.0f, Vec3{0.0f, 1.0f, 0.0f}};
    for (size_t i = 0; i != number_of_runs; ++i) {
        auto scatter_info = material.scatter(in_ray, record);
        REQUIRE(scatter_info.has_value());

        auto scattered_ray_direction = scatter_info->ray.direction();
        // Reflection will have the positive y coordinate.
        if (scattered_ray_direction.y() > 0) {
            // Discard reflection.
            continue;
        }
        // To get sin, we should get the x coordinate of scattered_ray_direction
        auto expected_sin_angle = 0.5f * std::sin(deg2rad(45.0f));
        REQUIRE(scattered_ray_direction.x() == expected_sin_angle);
        break;
        // Test refracted ray only once.
    }
}

TEST_CASE("When material 0.5 ref index, hit at 45 degree angle, it reflects", "[refractive]") {
    Random rand;
    RefractiveMaterial material(0.5, rand);
    Ray in_ray{Point3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, -1.0f, 0.0f}};
    Hittable::HitRecord record{Point3{1.0f, 0.0f, 0.0f}, in_ray, 1.0f, Vec3{0.0f, 1.0f, 0.0f}};
    for (size_t i = 0; i != number_of_runs; ++i) {
        auto scatter_info = material.scatter(in_ray, record);
        REQUIRE(scatter_info.has_value());

        auto scattered_ray_direction = scatter_info->ray.direction();
        REQUIRE(scattered_ray_direction == Vec3{1.0f, 1.0f, 0.0f}.unit());
    }
}

TEST_CASE("When material 2.0 ref index, hit almost parallel to the surface, almost always refracts", "[refractive]") {
    Random rand;
    RefractiveMaterial material(2.0, rand);
    Ray in_ray{Point3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, -1e-6f, 0.0f}};
    Hittable::HitRecord record{Point3{1.0f, 0.0f, 0.0f}, in_ray, 1.0f, Vec3{0.0f, 1.0f, 0.0f}};

    size_t number_of_refractions = 0;
    for (size_t i = 0; i != number_of_runs; ++i) {
        auto scatter_info = material.scatter(in_ray, record);
        REQUIRE(scatter_info.has_value());

        auto scattered_ray_direction = scatter_info->ray.direction();
        if (scattered_ray_direction.y() < 0) {
            // refracted here
            ++number_of_refractions;
            continue;
        }
        REQUIRE(scattered_ray_direction == Vec3{1.0f, 1e-6f, 0.0f}.unit());
    }
    REQUIRE(number_of_refractions < 3);
}

