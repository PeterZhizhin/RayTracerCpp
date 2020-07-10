//
// Created by Петр on 03.07.2020.
//

#ifndef RAYTRACERPROJECT_VEC3_H
#define RAYTRACERPROJECT_VEC3_H

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <ostream>
#include <fmt/format.h>
#include <fmt/ostream.h>

namespace ray_tracer::vector {

    template<typename Float>
    class Vec3Template {
    public:
        constexpr Vec3Template(Float x, Float y, Float z) : data_{x, y, z} {}

        constexpr Vec3Template() : Vec3Template(0, 0, 0) {}

        [[nodiscard]] constexpr Float x() const noexcept {
            return data_[0];
        }

        [[nodiscard]] constexpr Float y() const noexcept {
            return data_[1];
        }

        [[nodiscard]] constexpr Float z() const noexcept {
            return data_[2];
        }

        [[nodiscard]] Vec3Template operator-() const noexcept {
            return Vec3Template{-data_[0], -data_[1], -data_[2]};
        }

        [[nodiscard]] constexpr Float operator[](size_t index) const noexcept {
            return data_[index];
        }

        [[nodiscard]] constexpr Float &operator[](size_t index) noexcept {
            return data_[index];
        }

        constexpr Vec3Template &operator+=(const Vec3Template<Float> &other) noexcept {
            for (size_t i = 0; i != size_; ++i) {
                data_[i] += other[i];
            }
            return *this;
        }

        constexpr Vec3Template &operator-=(const Vec3Template<Float> &other) noexcept {
            for (size_t i = 0; i != size_; ++i) {
                data_[i] -= other[i];
            }
            return *this;
        }

        constexpr Vec3Template &operator*=(const Float multiplier) noexcept {
            for (size_t i = 0; i != size_; ++i) {
                data_[i] *= multiplier;
            }
            return *this;
        }

        constexpr Vec3Template &operator*=(const Vec3Template<Float>& other) noexcept {
            for (size_t i = 0; i != size_; ++i) {
                data_[i] *= other.data_[i];
            }
            return *this;
        }

        constexpr Vec3Template &operator/=(const Float divisor) noexcept {
            for (size_t i = 0; i != size_; ++i) {
                data_[i] /= divisor;
            }
            return *this;
        }

        constexpr bool operator==(const Vec3Template &other) const noexcept {
            return std::equal(data_.begin(), data_.end(), other.data_.begin(), other.data_.end());
        }

        constexpr bool operator!=(const Vec3Template &other) const noexcept {
            return !(*this == other);
        }

        [[nodiscard]] constexpr Float dot_product(const Vec3Template<Float> &other) const noexcept {
            Float result = 0;
            for (size_t i = 0; i != size_; ++i) {
                result += data_[i] * other[i];
            }
            return result;
        }

        [[nodiscard]] constexpr Float length2() const noexcept {
            return dot_product(*this);
        }

        [[nodiscard]] constexpr Float length() const noexcept {
            return std::sqrt(length2());
        }

        template<typename OtherType, typename MaxValueType>
        [[nodiscard]] Vec3Template<OtherType> cast(MaxValueType max_value) const noexcept {
            // std::nextafter is not constexpr :<
            const auto mult_val = std::nextafter(max_value, -std::numeric_limits<decltype(max_value)>::infinity());
            return {static_cast<OtherType>(mult_val * data_[0]), static_cast<OtherType>(mult_val * data_[1]),
                    static_cast<OtherType>(mult_val * data_[2])};
        }

        [[nodiscard]] constexpr Vec3Template unit() const noexcept {
            return *this / length();
        }

    private:
        std::array<Float, 3> data_;
        inline static constexpr size_t size_ = 3;
    };

    template<typename Float>
    [[nodiscard]] constexpr auto operator+(const Vec3Template<Float> &a, const Vec3Template<Float> &b) noexcept {
        Vec3Template<Float> result{a};
        return result += b;
    }

    template<typename Float>
    [[nodiscard]] constexpr auto operator-(const Vec3Template<Float> &a, const Vec3Template<Float> &b) noexcept {
        Vec3Template<Float> result{a};
        return result -= b;
    }

    template<typename Float>
    [[nodiscard]] constexpr auto operator*(const Vec3Template<Float> &a, const Vec3Template<Float> &b) noexcept {
        Vec3Template<Float> result{a};
        return result *= b;
    }

    template<typename Float>
    [[nodiscard]] constexpr auto operator*(const Vec3Template<Float> &a, const Float b) noexcept {
        Vec3Template<Float> result{a};
        return result *= b;
    }

    template<typename Float>
    [[nodiscard]] constexpr auto operator*(const Float b, const Vec3Template<Float> &a) noexcept {
        Vec3Template<Float> result{a};
        return result *= b;
    }

    template<typename Float>
    [[nodiscard]] constexpr auto operator/(const Vec3Template<Float> &a, const Float b) noexcept {
        Vec3Template<Float> result{a};
        return result /= b;
    }

    template <typename Float>
    std::ostream& operator<<(std::ostream& os, const Vec3Template<Float>& a) noexcept {
        fmt::print(os, "[{}; {}; {}]", a.x(), a.y(), a.z());
        return os;
    }

    using Vec3 = Vec3Template<float>;
    using Color3 = Vec3;
    using Point3 = Vec3;
}


#endif //RAYTRACERPROJECT_VEC3_H
