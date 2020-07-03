#include <absl/flags/flag.h>     // for GetFlag, ABSL_FLAG
#include <absl/flags/parse.h>    // for ParseCommandLine
#include <array>                 // for array
#include <cmath>                 // for lerp
#include <cstdint>               // for uint32_t
#include <exception>
#include <spdlog/spdlog.h>       // for error
#include <string>                // for string, allocator, basic_string
#include "image.h"
#include "vec3.h"
#include "ray.h"
#include "sphere.h"

ABSL_FLAG(std::string, file, "output.ppm", "Output file path");
ABSL_FLAG(uint32_t, width, 256, "Output image width");
ABSL_FLAG(uint32_t, height, 256, "Output image height");

using ray_tracer::ray::Ray;
using ray_tracer::vector::Color3;
using ray_tracer::vector::Point3;
using ray_tracer::vector::Vec3;
using ray_tracer::geometry::Sphere;

[[nodiscard]] Color3 get_ray_color(const Ray &ray) noexcept {
    Sphere sphere{{0.0f, 0.0f, -1.0f}, 0.5f};
    auto hit_or_none = sphere.hit(ray, 0, 100);
    if (hit_or_none) {
        const auto& hit_record = hit_or_none.value();
        return (hit_record.normal_at_hit + Vec3{1.0f, 1.0f, 1.0f}) * 0.5f;
    }
    const auto unit_ray_direction = ray.direction().unit();
    const auto t = 0.5f * (unit_ray_direction.y() + 1.0f);
    return (1.0f - t) * Color3{1.0f, 1.0f, 1.0f} + t * Color3{0.5f, 0.7f, 1.0f};
}

[[nodiscard]] auto
get_simple_image(const uint32_t image_width, const uint32_t image_height) noexcept {
    ray_tracer::Image result(image_height, image_width);

    const auto aspect_ratio = static_cast<float>(image_width) / static_cast<float>(image_height);
    const auto viewport_height = 2.0f;
    const auto viewport_width = aspect_ratio * viewport_height;
    const auto focal_length = 1.0f;

    Point3 origin{0.0f, 0.0f, 0.0f};
    Vec3 horizontal{viewport_width, 0.0f, 0.0f};
    Vec3 vertical{0.0f, -viewport_height, 0.0f};

    auto lower_left_corner = origin - horizontal / 2.0f - vertical / 2.0f - Vec3{0.0, 0.0, focal_length};

    for (uint32_t height = 0; height != image_height; ++height) {
        for (uint32_t width = 0; width != image_width; ++width) {
            auto u = static_cast<float>(width) / (image_width - 1);
            auto v = static_cast<float>(height) / (image_height - 1);

            Ray uv_ray{origin, lower_left_corner + u * horizontal + v * vertical};
            result[height][width] = get_ray_color(uv_ray);
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
