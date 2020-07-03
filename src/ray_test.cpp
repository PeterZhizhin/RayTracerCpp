#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include "vec3.h"
#include "ray.h"

using ray_tracer::vector::Vec3;
using ray_tracer::vector::Point3;
using ray_tracer::ray::Ray;

TEST_CASE("Ray creation with default creates a ray with zero origin and zero direction", "[ray]") {
    Ray test_obj;
    REQUIRE(test_obj.origin()[0] == 0.0f);
    REQUIRE(test_obj.origin()[1] == 0.0f);
    REQUIRE(test_obj.origin()[2] == 0.0f);

    REQUIRE(test_obj.direction()[0] == 0.0f);
    REQUIRE(test_obj.direction()[1] == 0.0f);
    REQUIRE(test_obj.direction()[2] == 0.0f);
}

TEST_CASE("Ray creation with parameters creates an expected ray", "[ray]") {
    Ray test_obj{{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}};
    REQUIRE(test_obj.origin()[0] == 1.0f);
    REQUIRE(test_obj.origin()[1] == 2.0f);
    REQUIRE(test_obj.origin()[2] == 3.0f);

    REQUIRE(test_obj.direction()[0] == 4.0f);
    REQUIRE(test_obj.direction()[1] == 5.0f);
    REQUIRE(test_obj.direction()[2] == 6.0f);
}

TEST_CASE("Ray origin (0 0 0) direction (1 2 3) at 2.0 is (2 4 6)", "[ray]") {
    Ray test_obj{{0.0f, 0.0f, 0.0f}, {1.0f, 2.0f, 3.0f}};
    auto ray_at_2 = test_obj.at(2.0f);
    REQUIRE(ray_at_2[0] == 2.0f);
    REQUIRE(ray_at_2[1] == 4.0f);
    REQUIRE(ray_at_2[2] == 6.0f);
}

TEST_CASE("Ray origin (1 2 3) direction (1 2 3) at -2.0 is (-1 -2 -3)", "[ray]") {
    Ray test_obj{{1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}};
    auto ray_at_2 = test_obj.at(-2.0f);
    REQUIRE(ray_at_2[0] == -1.0f);
    REQUIRE(ray_at_2[1] == -2.0f);
    REQUIRE(ray_at_2[2] == -3.0f);
}
