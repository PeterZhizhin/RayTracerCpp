#define CATCH_CONFIG_MAIN // This tells the catch header to generate a main

#include <sstream>
#include <catch2/catch.hpp>
#include <cmath>
#include "vec3.h"

using ray_tracer::vector::Vec3;

TEST_CASE("Vec3 default initialization returns zeros", "[vec3]") {
    Vec3 test_obj;
    REQUIRE(test_obj[0] == 0);
    REQUIRE(test_obj[1] == 0);
    REQUIRE(test_obj[2] == 0);
}

TEST_CASE("Vec3 initialization with values returns expected values", "[vec3]") {
    Vec3 test_obj{1.0f, 2.0f, 3.0f};
    REQUIRE(test_obj[0] == 1.0f);
    REQUIRE(test_obj[1] == 2.0f);
    REQUIRE(test_obj[2] == 3.0f);
}

TEST_CASE("Vec3 sum returns sum of vectors", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{10.0f, 20.0f, 30.0f};
    Vec3 c = a + b;
    REQUIRE(c[0] == 11.0f);
    REQUIRE(c[1] == 22.0f);
    REQUIRE(c[2] == 33.0f);
}

TEST_CASE("Vec3 += returns plus equal value", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{10.0f, 20.0f, 30.0f};
    a += b;
    REQUIRE(a[0] == 11.0f);
    REQUIRE(a[1] == 22.0f);
    REQUIRE(a[2] == 33.0f);
}

TEST_CASE("Vec3 subtract returns difference of vectors", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{10.0f, 20.0f, 30.0f};
    Vec3 c = a - b;
    REQUIRE(c[0] == -9.0f);
    REQUIRE(c[1] == -18.0f);
    REQUIRE(c[2] == -27.0f);
}

TEST_CASE("Vec3 -= returns plus equal value", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{10.0f, 20.0f, 30.0f};
    a -= b;
    REQUIRE(a[0] == -9.0f);
    REQUIRE(a[1] == -18.0f);
    REQUIRE(a[2] == -27.0f);
}

TEST_CASE("Vec3 operator- returns minus value", "[vec3]") {
    Vec3 test_obj{1.0f, -2.0f, 3.0f};
    auto other = -test_obj;
    REQUIRE(other[0] == -1.0f);
    REQUIRE(other[1] == 2.0f);
    REQUIRE(other[2] == -3.0f);
}

TEST_CASE("Vec3 multiply by scalar returns scaled vector", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    auto test_obj = 10.0f * a;
    REQUIRE(test_obj[0] == 10.0f);
    REQUIRE(test_obj[1] == 20.0f);
    REQUIRE(test_obj[2] == 30.0f);
}

TEST_CASE("Vec3 *= by scalar returns scaled vector", "[vec3]") {
    Vec3 test_obj{1.0f, 2.0f, 3.0f};
    test_obj *= 10.0f;
    REQUIRE(test_obj[0] == 10.0f);
    REQUIRE(test_obj[1] == 20.0f);
    REQUIRE(test_obj[2] == 30.0f);
}

TEST_CASE("Vec3 *= by vector returns vector multiplication", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{1.0f, 10.0f, 100.0f};
    a *= b;
    REQUIRE(a == Vec3{1.0f, 20.0f, 300.0f});
}

TEST_CASE("Vec3 * by vector returns vector multiplication", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{1.0f, 10.0f, 100.0f};
    REQUIRE(a * b == Vec3{1.0f, 20.0f, 300.0f});
}

TEST_CASE("Vec3 divide by scalar returns scaled vector", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    auto test_obj = a / 10.0f;
    REQUIRE(test_obj[0] == 0.1f);
    REQUIRE(test_obj[1] == 0.2f);
    REQUIRE(test_obj[2] == 0.3f);
}

TEST_CASE("Vec3 /= by scalar returns scaled vector", "[vec3]") {
    Vec3 test_obj{1.0f, 2.0f, 3.0f};
    test_obj /= 10.0f;
    REQUIRE(test_obj[0] == 0.1f);
    REQUIRE(test_obj[1] == 0.2f);
    REQUIRE(test_obj[2] == 0.3f);
}


TEST_CASE("Vec3 dot product returns expected dot product", "[vec3]") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{100.0f, 10.0f, 1.0f};
    REQUIRE(a.dot_product(b) == 123.0f);
}

TEST_CASE("Vec3 length2 returns squared length", "[vec3]") {
    Vec3 a{100.0f, 10.0f, 1.0f};
    REQUIRE(a.length2() == 10101.0f);
}

TEST_CASE("Vec3 length returns length", "[vec3]") {
    Vec3 a{3.0f, 4.0f, 0.0f};
    REQUIRE(a.length() == 5.0f);
}

TEST_CASE("Vec3 casting to int returns expected int", "[vec3]") {
    Vec3 a{0.0f, 0.5f, 1.0f};
    auto result = a.cast<uint8_t>(256.0f);
    REQUIRE(result[0] == 0);
    REQUIRE(result[1] == 127);
    REQUIRE(result[2] == 255);
}

TEST_CASE("Vec3 unit for (1 -1 1) returns ~(0.57735 -0.57735 0.57735)", "[vec3]") {
    Vec3 a{1.0f, -1.0f, 1.0f};
    auto unit_a = a.unit();
    REQUIRE(std::abs(unit_a[0] - 0.57735f) < 1e-5f);
    REQUIRE(std::abs(unit_a[1] + 0.57735f) < 1e-5f);
    REQUIRE(std::abs(unit_a[2] - 0.57735f) < 1e-5f);
}

TEST_CASE("Vec3 equality operator returns true on equal elements", "[vec3]") {
    Vec3 a{1.0f, -1.0f, 1.0f};
    Vec3 b{1.0f, -1.0f, 1.0f};
    REQUIRE(a == b);
}

TEST_CASE("Vec3 equality operator returns false on unequal elements", "[vec3]") {
    Vec3 a{1.0f, -1.0f, 1.0f};
    Vec3 b{1.0f, -1.0f, 0.0f};
    REQUIRE_FALSE(a == b);
}

TEST_CASE("Vec3 different vectors operator != returns true", "[vec3]") {
    Vec3 a{1.0f, -1.0f, 1.0f};
    Vec3 b{1.0f, -1.0f, 0.0f};
    REQUIRE(a != b);
}

TEST_CASE("Vec3 same vectors operator != returns false", "[vec3]") {
    Vec3 a{1.0f, -1.0f, 1.0f};
    Vec3 b{1.0f, -1.0f, 1.0f};
    REQUIRE_FALSE(a != b);
}

TEST_CASE("Vec3 outputs to ostream as expected", "[vec3]") {
    std::ostringstream output;
    Vec3 a{1.0f, 10.0f, 1.0f};
    output << a;
    REQUIRE(output.str() == "[1.0; 10.0; 1.0]");
}