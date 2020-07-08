#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <cstdint>
#include <exception>
#include <fmt/format.h>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <utility>

#include "tqdm/tqdm.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "image.h"
#include "random.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

ABSL_FLAG(std::string, file, "output.ppm", "Output file path");
ABSL_FLAG(uint32_t, width, 256, "Output image width");
ABSL_FLAG(uint32_t, height, 256, "Output image height");
ABSL_FLAG(uint32_t, rays_per_pixel, 8, "Number of rays per pixel (for antialiasing)");
ABSL_FLAG(uint32_t, max_depth, 50, "Maximum depth for tracing rays");

using ray_tracer::geometry::Hittable;
using ray_tracer::geometry::HittableList;
using ray_tracer::geometry::Sphere;
using ray_tracer::ray::Ray;
using ray_tracer::vector::Color3;
using ray_tracer::vector::Point3;
using ray_tracer::vector::Vec3;
using ray_tracer::camera::Camera;
using ray_tracer::random::Random;

constexpr auto lambertian_reflectance(const Vec3 &normal_at_hit, const Vec3 &random_on_unit_sphere) {
    auto target_direction = normal_at_hit + random_on_unit_sphere;
    // color_multiplier = <target / target.length(); normal> = <target; normal> / target.length() =
    // = <normal + random; normal> / target.length() = (<normal; normal> + <random; normal>) / target.length() =
    // = [normal.length() == 1, target.length() == sqrt(<target; target>)] =
    // = (1 + <random; normal>) / sqrt(<normal + random; normal + random>) =
    // = (1 + <random; normal>) / sqrt(<normal; normal> + 2<normal; random> + <random; random>) =
    // = [random.length() == 1] = (1 + <random; normal>) / sqrt(1 + 2<normal; random> + 1) =
    // = (1 + <random; normal>) / sqrt(2) / sqrt(1 + <random; normal>) = sqrt((1 + <random; normal>) / 2)
    auto color_multiplier = std::sqrt((1 + random_on_unit_sphere.dot_product(normal_at_hit)) / 2.0f);
    return std::make_pair(target_direction, color_multiplier);
}

[[nodiscard]] Color3
get_ray_color(const Ray &ray, const Hittable &hittable, Random &random, const uint32_t depth_quota) noexcept {
    if (depth_quota == 0) {
        // Cannot go further anymore.
        return Color3{};
    }

    auto hit_or_none = hittable.hit(ray, 1e-4f, 100);
    if (hit_or_none) {
        const auto &hit_record = hit_or_none.value();
        auto[target_direction, color_multiplier] = lambertian_reflectance(hit_record.normal_at_hit,
                                                                          random.on_unit_sphere_uniform());
        Ray target_ray{hit_record.hit_point, target_direction};
        return color_multiplier * get_ray_color(target_ray, hittable, random,
                                                depth_quota - 1);
    }
    const auto unit_ray_direction = ray.direction().unit();
    const auto t = 0.5f * (unit_ray_direction.y() + 1.0f);
    return (1.0f - t) * Color3{1.0f, 1.0f, 1.0f} + t * Color3{0.5f, 0.7f, 1.0f};
}

[[nodiscard]] auto
get_simple_image(const uint32_t image_width, const uint32_t image_height, const uint32_t rays_per_pixel,
                 const uint32_t max_ray_tracing_depth) noexcept {
    ray_tracer::Image result(image_height, image_width);

    Camera camera(image_width, image_height);

    HittableList hittable_list;
    hittable_list.add(std::make_unique<Sphere>(Point3{0.0f, 0.0f, -1.0f}, 0.5f));
    hittable_list.add(std::make_unique<Sphere>(Point3{0.0f, -100.5f, -1.0f}, 100.0f));

    Random random;

    tqdm::tqdm bar;
    for (uint32_t height = 0; height != image_height; ++height) {
        bar.progress(static_cast<int>(height), static_cast<int>(image_height));
        for (uint32_t width = 0; width != image_width; ++width) {
            Color3 colors_sum;
            for (uint32_t ray_no = 0; ray_no != rays_per_pixel; ++ray_no) {
                auto u = (static_cast<float>(width) + random.standard_uniform()) / static_cast<float>(image_width - 1);
                auto v = 1.0f - (static_cast<float>(height) + random.standard_uniform()) / static_cast<float>(
                        image_height - 1);

                auto uv_ray = camera.get_ray(u, v);
                colors_sum += get_ray_color(uv_ray, hittable_list, random, max_ray_tracing_depth);
            }
            auto non_gamma_corrected = colors_sum / static_cast<float>(rays_per_pixel);
            Color3 gamma_corrected{std::sqrt(non_gamma_corrected.x()), std::sqrt(non_gamma_corrected.y()),
                                   std::sqrt(non_gamma_corrected.z())};
            result[height][width] = gamma_corrected;
        }
    }
    bar.finish();
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
    const auto rays_per_pixel = absl::GetFlag(FLAGS_rays_per_pixel);
    const auto max_ray_tracing_depth = absl::GetFlag(FLAGS_max_depth);
    if (width == 0 || width > 2048) {
        spdlog::error("Image width is invalid (valid range is (0; 2048]), got {}", width);
        return 1;
    }
    if (height == 0 || height > 2048) {
        spdlog::error("Image height is invalid (valid range is (0; 2048]), got {}", height);
        return 1;
    }
    if (rays_per_pixel == 0) {
        spdlog::error("Number of rays is invalid (valid range is anything >0), got {}", rays_per_pixel);
        return 1;
    }

    try {
        auto image = get_simple_image(width, height, rays_per_pixel, max_ray_tracing_depth);
        image.save_image(output_file_path, ray_tracer::OutputFormat::PNG);
    } catch (const std::exception &exception) {
        spdlog::error("Error on saving image:\n{}\n", exception.what());
        return 1;
    }
}
