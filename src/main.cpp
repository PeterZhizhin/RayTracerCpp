#include <absl/flags/flag.h>
#include <absl/flags/parse.h>
#include <array>
#include <cmath>
#include <cstdint>
#include <fmt/core.h> // for print
#include <fmt/ostream.h> // for print
#include <fstream>
#include <iostream>
#include <spdlog/spdlog.h> // for info
#include <string_view>
#include <tuple>


ABSL_FLAG(std::string, file, "", "Output file path (stdout by default)");
ABSL_FLAG(uint32_t, width, 256, "Output image width");
ABSL_FLAG(uint32_t, height, 256, "Output image height");

static constexpr int colors = 256;

using color_range_t = std::array<double, 2>;
static constexpr color_range_t red_range = { 0.0, 0.99999 };
static constexpr color_range_t green_range = { 0.0, 0.99999 };
static constexpr color_range_t blue_range = { 0.25, 0.25 };

template<typename FloatType>
constexpr int float_color_to_int(const FloatType c)
{
  int ic = static_cast<int>(colors * c);
  return ic;
}

template<typename FloatType>
constexpr std::tuple<int, int, int> float_color_to_int(const FloatType r, const FloatType g, const FloatType b)
{
  int ir = float_color_to_int(r);
  int ig = float_color_to_int(g);
  int ib = float_color_to_int(b);
  return { ir, ig, ib };
}

void print_simple_image(const uint32_t image_width, const uint32_t image_height, std::ostream *output_stream)
{
  fmt::print(*output_stream, "P3\n{} {}\n{}\n", image_width, image_height, colors - 1);
  for (uint32_t height = 0; height != image_height; ++height) {
    spdlog::info("Progress: {}/{} lines.", height, image_height);
    for (uint32_t width = 0; width != image_width; ++width) {
      auto rt = static_cast<double>(width) / (image_width - 1);
      auto gt = static_cast<double>(height) / (image_height - 1);
      auto bt = 0.0;

      auto r = std::lerp(red_range[0], red_range[1], rt);
      auto g = std::lerp(green_range[0], green_range[1], gt);
      auto b = std::lerp(blue_range[0], blue_range[1], bt);

      auto [ir, ig, ib] = float_color_to_int(r, g, b);
      fmt::print(*output_stream, "{} {} {}\n", ir, ig, ib);
    }
  }
}

int main(int argc, char *argv[])
{
  absl::ParseCommandLine(argc, argv);
  const auto output_file_path = absl::GetFlag(FLAGS_file);
  std::ostream *output_stream = &std::cout;
  std::ofstream output_file;
  if (!output_file_path.empty()) {
    spdlog::info("Outputting to {}", output_file_path);
    output_file.open(output_file_path);
    output_stream = &output_file;
  }

  const auto width = absl::GetFlag(FLAGS_width);
  const auto height = absl::GetFlag(FLAGS_height);

  print_simple_image(width, height, output_stream);
}
