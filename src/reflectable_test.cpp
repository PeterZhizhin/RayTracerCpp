#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <cmath>
#include "hittable.h"
#include "ray.h"
#include "reflectable.h"
#include "vec3.h"

using ray_tracer::geometry::Hittable;
using ray_tracer::material::ReflectableMaterial;
using ray_tracer::ray::Ray;
using ray_tracer::vector::Color3;
using ray_tracer::vector::Point3;
using ray_tracer::vector::Vec3;

TEST_CASE("When hit at 45 degree angle reflects at 45 degree", "[reflectable]") {
    ReflectableMaterial material{Color3{}};
    Ray in_ray{Point3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, -1.0f, 0.0f}};
    Hittable::HitRecord hit_record{Point3{1.0f, 0.0f, 0.0f}, in_ray, 1.0f, Vec3{0.0f, 1.0f, 0.0f}};

    auto scattered = material.scatter(in_ray, hit_record);
    REQUIRE(scattered.has_value());
    auto scattered_ray = scattered->ray;
    REQUIRE(scattered_ray.direction() == Vec3{1.0f, 1.0f, 0.0f}.unit());
}

TEST_CASE("When hit, returns material color", "[reflectable]") {
    Color3 material_color{0.8f, 0.5f, 1.0f};
    ReflectableMaterial material{material_color};
    Ray in_ray{Point3{0.0f, 1.0f, 0.0f}, Vec3{1.0f, -1.0f, 0.0f}};
    Hittable::HitRecord hit_record{Point3{1.0f, 0.0f, 0.0f}, in_ray, 1.0f, Vec3{0.0f, 1.0f, 0.0f}};

    auto scattered = material.scatter(in_ray, hit_record);
    REQUIRE(scattered.has_value());
    auto color = scattered->attenuation;
    REQUIRE(color == material_color);
}
