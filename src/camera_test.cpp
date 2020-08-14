#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <cmath>
#include "camera.h"
#include "ray.h"
#include "vec3.h"

using ray_tracer::ray::Ray;
using ray_tracer::vector::Point3;
using ray_tracer::camera::Camera;
using ray_tracer::random::Random;

TEST_CASE("Camera get ray returns origin (0 0 0)", "[camera]") {
    Random random;
    Camera camera{16, 9, random};
    auto ray = camera.get_ray(0.0f, 0.0f);
    REQUIRE(ray.origin() == Point3{0.0f, 0.0f, 0.0f});
}

TEST_CASE("Camera get ray at (0.5 0.5) returns ray directed as (0 0 -focal_len)", "[camera]") {
    Random random;
    Camera camera{16, 9, random,
            /* vertical_field_of_view = */ static_cast<float>(M_PI_2),
            /* lookfrom = */ {},
            /* lookat = */ {0.0f, 0.0f, -1.0f},
            /* vector_up = */ {0.0f, 1.0f, 0.0f}};
    auto ray = camera.get_ray(0.5f, 0.5f);
    REQUIRE(ray.direction() == Point3{0.0f, 0.0f, -1.0f});
}

TEST_CASE("Camera get ray at (1 1) returns ray directed as (16 / 9; 1; -focal_len) when vfov == 90deg",
          "[camera]") {
    Random random;
    Camera camera{16, 9, random,
            /* vertical_field_of_view = */ static_cast<float>(M_PI_2),
            /* lookfrom = */ {},
            /* lookat = */ {0.0f, 0.0f, -1.0f},
            /* vector_up = */ {0.0f, 1.0f, 0.0f}};
    auto ray = camera.get_ray(1.0f, 1.0f);
    REQUIRE(ray.direction() == Point3{16.0f / 9.0f, 1.0f, -1.0f});
}

TEST_CASE("Camera get ray at (0.5 1) returns ray directed as (0; 1; -focal_len) when vfov == 90deg",
          "[camera]") {
    Random random;
    Camera camera{16, 9, random,
            /* vertical_field_of_view = */ static_cast<float>(M_PI_2),
            /* lookfrom = */ {},
            /* lookat = */ {0.0f, 0.0f, -1.0f},
            /* vector_up = */ {0.0f, 1.0f, 0.0f}};
    auto ray = camera.get_ray(0.5f, 1.0f);
    REQUIRE(ray.direction() == Point3{0.0f, 1.0f, -1.0f});
}

TEST_CASE("Camera get ray at (1 0.5) returns ray directed as (16 / 9; 0; -focal_len) when vfov == 90",
          "[camera]") {
    Random random;
    Camera camera{16, 9, random, /* vertical_field_of_view = */ static_cast<float>(M_PI_2)};
    auto ray = camera.get_ray(1.0f, 0.5f);
    REQUIRE(ray.direction() == Point3{16.0f / 9.0f, 0.0f, -1.0f});
}

TEST_CASE("Camera get ray at (0.5 0.5) with aperture == 2 always returns ray directed at lookat point",
          "[camera]") {
    Random random;
    Camera camera{16, 9, random,
            /* vertical_field_of_view = */ static_cast<float>(M_PI_2),
            /* lookfrom = */ {},
            /* lookat = */ {0.0f, 0.0f, -1.0f},
            /* vector_up = */ {0.0f, 1.0f, 0.0f},
            /* aperture = */ 2.0f,
            /* focus_dist = */ 1.0f};
    for (size_t i = 0; i != 1000; ++i) {
        auto ray = camera.get_ray(0.5f, 0.5f);
        auto endpoint = ray.origin() + ray.direction();
        REQUIRE(endpoint == Point3{0.0f, 0.0f, -1.0f});
    }
}

TEST_CASE("Camera get ray at (0.5 0.5) with aperture == 0 with focal distance 1337 returns (0, 0, -1337)",
          "[camera]") {
    Random random;
    Camera camera{16, 9, random,
            /* vertical_field_of_view = */ static_cast<float>(M_PI_2),
            /* lookfrom = */ {},
            /* lookat = */ {0.0f, 0.0f, -1.0f},
            /* vector_up = */ {0.0f, 1.0f, 0.0f},
            /* aperture = */ 0.0f,
            /* focus_dist = */ 1337.0f};
    auto ray = camera.get_ray(0.5f, 0.5f);
    REQUIRE(ray.direction() == Point3{0.0f, 0.0f, -1337.0f});
}
