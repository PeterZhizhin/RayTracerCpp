#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <cmath>
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

using ray_tracer::vector::Vec3;
using ray_tracer::vector::Point3;
using ray_tracer::ray::Ray;
using ray_tracer::geometry::Hittable;

TEST_CASE("HitRecord initialization with ray same direction as normal returns non-front face and negative normal",
          "[hittable]") {
    Point3 hit{1.0f, 0.0f, 0.0f};
    Vec3 hit_outward_normal{1.0f, 0.0f, 0.0f};

    Point3 ray_origin{0.0f, 0.0f, 0.0f};
    Vec3 ray_direction{0.5f, 0.0f, 0.0f};

    float ray_t{2.0f};

    Hittable::HitRecord record{hit, Ray{ray_origin, ray_direction}, ray_t, hit_outward_normal};
    REQUIRE_FALSE(record.front_face);
    auto normal_facing_ray = record.normal_at_hit;
    REQUIRE(normal_facing_ray[0] == -1.0f);
    REQUIRE(normal_facing_ray[1] == 0.0f);
    REQUIRE(normal_facing_ray[2] == 0.0f);
}

TEST_CASE("HitRecord initialization with ray not same direction as normal returns front face and positive normal",
          "[hittable]") {
    Point3 hit{1.0f, 0.0f, 0.0f};
    Vec3 hit_outward_normal{1.0f, 0.0f, 0.0f};

    Point3 ray_origin{2.0f, 0.0f, 0.0f};
    Vec3 ray_direction{-0.5f, 0.0f, 0.0f};

    float ray_t{2.0f};

    Hittable::HitRecord record{hit, Ray{ray_origin, ray_direction}, ray_t, hit_outward_normal};
    REQUIRE(record.front_face);
    auto normal_facing_ray = record.normal_at_hit;
    REQUIRE(normal_facing_ray[0] == 1.0f);
    REQUIRE(normal_facing_ray[1] == 0.0f);
    REQUIRE(normal_facing_ray[2] == 0.0f);
}
