#include "image.h"               // for Color, float_color_to_int, save_image
#include <absl/flags/flag.h>     // for GetFlag, ABSL_FLAG
#include <absl/flags/parse.h>    // for ParseCommandLine
#include <array>                 // for array
#include <cmath>                 // for lerp
#include <cstdint>               // for uint32_t
#include <exception>
#include <spdlog/spdlog.h>       // for error
#include <string>                // for string, allocator, basic_string

ABSL_FLAG(std::string, file, "output.ppm", "Output file path");
ABSL_FLAG(uint32_t, width, 256, "Output image width");
ABSL_FLAG(uint32_t, height, 256, "Output image height");


using color_range_t = std::array<float, 2>;
static constexpr color_range_t red_range = {0.0, 1.0};
static constexpr color_range_t green_range = {0.0, 1.0};
static constexpr color_range_t blue_range = {0.25, 0.25};

auto
get_simple_image(const uint32_t image_width, const uint32_t image_height) noexcept {
    ray_tracer::Image result(image_height, image_width);
    for (uint32_t height = 0; height != image_height; ++height) {
        for (uint32_t width = 0; width != image_width; ++width) {
            float rt = static_cast<float>(width) / (image_width - 1);
            float gt = static_cast<float>(height) / (image_height - 1);
            float bt = 0.0f;

            auto r = std::lerp(red_range[0], red_range[1], rt);
            auto g = std::lerp(green_range[0], green_range[1], gt);
            auto b = std::lerp(blue_range[0], blue_range[1], bt);

            result[height][width] = {r, g, b};
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
