#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>
#include <optional>
#include <utility>
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"

using ray_tracer::geometry::HittableList;
using ray_tracer::geometry::Hittable;
using ray_tracer::ray::Ray;
using ray_tracer::vector::Vec3;
using ray_tracer::vector::Point3;

class MockHittable : public Hittable {
public:
    MAKE_CONST_MOCK3 (hit, std::optional<Hittable::HitRecord>(const Ray&, float, float), noexcept);
};


TEST_CASE("Hittable list returns false on empty list", "[hittable_list]") {
    HittableList hittable_list;
    Ray test_ray{Point3{}, Vec3{}};
    auto result = hittable_list.hit(test_ray, 0.0f, 100.0f);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE("Hittable list returns hit record when added object return hit record", "[hittable_list]") {
    using trompeloeil::_;
    auto hittable1 = std::make_unique<MockHittable>();
    Hittable::HitRecord test_record{Point3{}, Ray{Point3{}, Vec3{}}, 0.5f, Vec3{}};
    REQUIRE_CALL(*hittable1, hit(_, _, _))
    .RETURN(test_record);

    HittableList hittable_list;
    hittable_list.add(std::move(hittable1));

    auto result = hittable_list.hit(Ray{Point3{}, Vec3{}}, 0.0f, 1.0f);
    REQUIRE(result.has_value());
}

TEST_CASE("Hittable list calls added object hit with expected parameters and returns returned record",
          "[hittable_list]") {
    using trompeloeil::_;
    using trompeloeil::eq;
    auto hittable1 = std::make_unique<MockHittable>();

    Ray test_ray{Point3{}, Vec3{}};
    float t_min{0.0f};
    float t_max{1.0f};

    float ray_t{0.5f};
    Point3 hit_point{1337.0f, 1337.0f, 1337.0f};
    Hittable::HitRecord test_record{hit_point, test_ray, ray_t, Vec3{}};
    REQUIRE_CALL(*hittable1, hit(_, eq(t_min), eq(t_max)))
            .LR_WITH(&_1 == &test_ray)
            .RETURN(test_record);

    HittableList hittable_list;
    hittable_list.add(std::move(hittable1));

    auto result = hittable_list.hit(test_ray, t_min, t_max);
    REQUIRE(result.has_value());
    REQUIRE(result->ray_t == ray_t);
    REQUIRE(result->hit_point == hit_point);
}

TEST_CASE("Hittable list returns false when added object returns nullopt",
          "[hittable_list]") {
    using trompeloeil::_;
    auto hittable1 = std::make_unique<MockHittable>();
    REQUIRE_CALL(*hittable1, hit(_, _, _))
    .RETURN(std::nullopt);

    HittableList hittable_list;
    hittable_list.add(std::move(hittable1));

    auto result = hittable_list.hit(Ray{Point3{}, Vec3{}}, 0.0f, 1000.0f);
    REQUIRE_FALSE(result.has_value());
}

TEST_CASE(
        "Hittable list returns HitRecord when 2nd added object returns record and 1st, 3rd return nullopt",
        "[hittable_list]") {
    using trompeloeil::_;
    auto hittable1 = std::make_unique<MockHittable>();
    auto hittable2 = std::make_unique<MockHittable>();
    auto hittable3 = std::make_unique<MockHittable>();
    ALLOW_CALL(*hittable1, hit(_, _, _))
    .RETURN(std::nullopt);

    Hittable::HitRecord test_record{Point3{}, Ray{Point3{}, Vec3{}}, 0.5f, Vec3{}};
    REQUIRE_CALL(*hittable2, hit(_, _, _))
    .RETURN(test_record);

    ALLOW_CALL(*hittable3, hit(_, _, _))
    .RETURN(std::nullopt);

    HittableList hittable_list;
    hittable_list.add(std::move(hittable1));
    hittable_list.add(std::move(hittable2));
    hittable_list.add(std::move(hittable3));

    auto result = hittable_list.hit(Ray{Point3{}, Vec3{}}, 0.0f, 1000.0f);
    REQUIRE(result.has_value());
}

TEST_CASE(
        "Hittable list returns closest HitRecord when 3 objects return hit records, 3rd closest",
        "[hittable_list]") {
    using trompeloeil::_;
    auto hittable1 = std::make_unique<MockHittable>();
    auto hittable2 = std::make_unique<MockHittable>();
    auto hittable3 = std::make_unique<MockHittable>();

    // The record is very far
    Hittable::HitRecord test_record1{Point3{}, Ray{Point3{}, Vec3{}}, 100.0f, Vec3{}};
    // The record is a very close
    Hittable::HitRecord test_record2{Point3{}, Ray{Point3{}, Vec3{}}, 0.5f, Vec3{}};
    // The record is the closest
    Hittable::HitRecord test_record3{Point3{}, Ray{Point3{}, Vec3{}}, 0.01f, Vec3{}};

    ALLOW_CALL(*hittable1, hit(_, _, _))
    .RETURN(test_record1);

    ALLOW_CALL(*hittable2, hit(_, _, _))
    .RETURN(test_record2);

    ALLOW_CALL(*hittable3, hit(_, _, _))
    .RETURN(test_record3);

    HittableList hittable_list;
    hittable_list.add(std::move(hittable1));
    hittable_list.add(std::move(hittable2));
    hittable_list.add(std::move(hittable3));

    auto result = hittable_list.hit(Ray{Point3{}, Vec3{}}, 0.0f, 1000.0f);
    REQUIRE(result.has_value());
    REQUIRE(result->ray_t == 0.01f);
}
