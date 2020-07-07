#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <cstdint>
#include <exception>
#include <fmt/format.h>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "image.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

ABSL_FLAG(std::string, file, "output.ppm", "Output file path");
ABSL_FLAG(uint32_t, width, 256, "Output image width");
ABSL_FLAG(uint32_t, height, 256, "Output image height");

using ray_tracer::geometry::Hittable;
using ray_tracer::geometry::HittableList;
using ray_tracer::geometry::Sphere;
using ray_tracer::ray::Ray;
using ray_tracer::vector::Color3;
using ray_tracer::vector::Point3;
using ray_tracer::vector::Vec3;
using ray_tracer::camera::Camera;

[[nodiscard]] Color3 get_ray_color(const Ray &ray, const Hittable& hittable) noexcept {
    auto hit_or_none = hittable.hit(ray, 0, 100);
    if (hit_or_none) {
        const auto &hit_record = hit_or_none.value();
        return (hit_record.normal_at_hit + Vec3{1.0f, 1.0f, 1.0f}) * 0.5f;
    }
    const auto unit_ray_direction = ray.direction().unit();
    const auto t = 0.5f * (unit_ray_direction.y() + 1.0f);
    return (1.0f - t) * Color3{1.0f, 1.0f, 1.0f} + t * Color3{0.5f, 0.7f, 1.0f};
}

[[nodiscard]] auto
get_simple_image(const uint32_t image_width, const uint32_t image_height) noexcept {
    ray_tracer::Image result(image_height, image_width);

    Camera camera(image_width, image_height);

    HittableList hittable_list;
    hittable_list.add(std::make_unique<Sphere>(Point3{0.0f, 0.0f, -1.0f}, 0.5f));
    hittable_list.add(std::make_unique<Sphere>(Point3{0.0f, -100.5f, -1.0f}, 100.0f));

    for (uint32_t height = 0; height != image_height; ++height) {
        for (uint32_t width = 0; width != image_width; ++width) {
            auto u = static_cast<float>(width) / static_cast<float>(image_width - 1);
            auto v = 1.0f - static_cast<float>(height) / static_cast<float>(image_height - 1);

            auto uv_ray = camera.get_ray(u, v);
            result[height][width] = get_ray_color(uv_ray, hittable_list);
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    absl::ParseCommandLine(argc, argv);
    const auto output_file_path = absl::GetFlag(FLAGS_file);
    if (output_file_path.empty()) {
        spdlog::error("Empty output file path!\n");
        return 1;
    }

    const auto width = absl::GetFlag(FLAGS_width);
    const auto height = absl::GetFlag(FLAGS_height);
    if (width == 0 || width > 2048) {
        spdlog::error("Image width is invalid (valid range is (0; 2048]), got {}", width);
        return 1;
    }
    if (height == 0 || height > 2048) {
        spdlog::error("Image height is invalid (valid range is (0; 2048]), got {}", height);
        return 1;
    }

    try {
        auto image = get_simple_image(width, height);
        image.save_image(output_file_path, ray_tracer::OutputFormat::PNG);
    } catch (const std::exception &exception) {
        spdlog::error("Error on saving image:\n{}\n", exception.what());
        return 1;
    }
}
