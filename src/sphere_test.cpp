#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <cmath>
#include "sphere.h"
#include "ray.h"
#include "vec3.h"

using ray_tracer::vector::Vec3;
using ray_tracer::vector::Point3;
using ray_tracer::ray::Ray;
using ray_tracer::geometry::Sphere;

TEST_CASE("Sphere center (0 0 0) radius 1 ray directed to (0 0 0) from (1 1 1).unit() hits sphere at 0", "[sphere]") {
    Sphere test_sphere{{0.0f, 0.0f, 0.0f}, 1.0f};

    Ray test_ray{Vec3{1.0f, 1.0f, 1.0f}.unit(), -Vec3{1.0f, 1.0f, 1.0f}.unit()};
    auto hit_obj = test_sphere.hit(test_ray, 0.0f, 1.0f);
    REQUIRE(hit_obj.has_value());
    REQUIRE(hit_obj->ray_t == 0.0f);
}

TEST_CASE("Sphere center (0 0 0) radius 1 ray directed to (0 0 0) from (2 0 0) hits sphere with normal (1 0 0)",
          "[sphere]") {
    Sphere test_sphere{{0.0f, 0.0f, 0.0f}, 1.0f};

    Ray test_ray{{2.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}};
    auto hit_obj = test_sphere.hit(test_ray, 0.0f, 10.0f);
    REQUIRE(hit_obj.has_value());
    REQUIRE(hit_obj->normal_at_hit[0] == 1.0f);
    REQUIRE(hit_obj->normal_at_hit[1] == 0.0f);
    REQUIRE(hit_obj->normal_at_hit[2] == 0.0f);
}

TEST_CASE("Sphere center (0 0 0) radius 1 ray directed to (0 0 0) from (0.5 0.5 0.5) hits sphere",
          "[sphere]") {
    Sphere test_sphere{{0.0f, 0.0f, 0.0f}, 1.0f};

    Ray test_ray{{0.5f, 0.5f, 0.5f}, {-1.0f, 0.0f, 0.0f}};
    auto hit_obj = test_sphere.hit(test_ray, 0.0f, 10.0f);
    REQUIRE(hit_obj.has_value());
}

TEST_CASE("Sphere center (0 0 0) radius 1 ray directed to (0 0.5 0.5) from (1.5 1.5 1.5) hits sphere",
          "[sphere]") {
    Sphere test_sphere{{0.0f, 0.0f, 0.0f}, 1.0f};

    Vec3 ray_center{1.5f, 1.5f, 1.5f};
    Vec3 ray_direction = (Vec3{0.0f, 0.5f, 0.5f} - ray_center).unit();
    Ray test_ray{ray_center, ray_direction};
    auto hit_obj = test_sphere.hit(test_ray, 0.0f, 10.0f);
    REQUIRE(hit_obj.has_value());
}

TEST_CASE("Sphere center (5 5 5) radius 2 ray directed to (0 0 0) from (1 1 1) does not hit",
          "[sphere]") {
    Sphere test_sphere{{5.0f, 5.0f, 5.0f}, 2.0f};

    Ray test_ray{{1.0f, 1.0f, 1.0f}, -Vec3{1.0f, 1.0f, 1.0f}.unit()};
    auto hit_obj = test_sphere.hit(test_ray, 0.0f, 10.0f);
    REQUIRE_FALSE(hit_obj.has_value());
}

