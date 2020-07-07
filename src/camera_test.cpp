#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <cmath>
#include "camera.h"
#include "ray.h"
#include "vec3.h"

using ray_tracer::ray::Ray;
using ray_tracer::vector::Point3;
using ray_tracer::camera::Camera;

TEST_CASE("Camera get ray returns origin (0 0 0)", "[camera]") {
    Camera camera{16, 9};
    auto ray = camera.get_ray(0.0f, 0.0f);
    REQUIRE(ray.origin() == Point3{0.0f, 0.0f, 0.0f});
}

TEST_CASE("Camera get ray at (0.5 0.5) returns ray directed as (0 0 -focal_len)", "[camera]") {
    Camera camera{16, 9, /* focal_length = */ 1337.0f};
    auto ray = camera.get_ray(0.5f, 0.5f);
    REQUIRE(ray.direction() == Point3{0.0f, 0.0f, -1337.0f});
}

TEST_CASE("Camera get ray at (1 1) returns ray directed as (viewport_width / 2; viewport_height / 2; -focal_len)",
          "[camera]") {
    Camera camera{16, 9, /* focal_length = */ 1337.0f, /* viewport_height = */ 9.0f};
    auto ray = camera.get_ray(1.0f, 1.0f);
    REQUIRE(ray.direction() == Point3{8.0f, 4.5f, -1337.0f});
}

TEST_CASE("Camera get ray at (0.5 1) returns ray directed as (0; viewport_height / 2; -focal_len)",
          "[camera]") {
    Camera camera{16, 9, /* focal_length = */ 1337.0f, /* viewport_height = */ 9.0f};
    auto ray = camera.get_ray(0.5f, 1.0f);
    REQUIRE(ray.direction() == Point3{0.0f, 4.5f, -1337.0f});
}

TEST_CASE("Camera get ray at (1 0.5) returns ray directed as (viewport_width / 2; 0; -focal_len)",
          "[camera]") {
    Camera camera{16, 9, /* focal_length = */ 1337.0f, /* viewport_height = */ 9.0f};
    auto ray = camera.get_ray(1.0f, 0.5f);
    REQUIRE(ray.direction() == Point3{8.0f, 0.0f, -1337.0f});
}
