#include "image.h"

#include <fmt/ostream.h>
#include <fstream>
#include <string>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#include "vec3.h"

namespace ray_tracer {
    namespace {
        inline constexpr float MAX_COLOR_VALUE = 256.0f;
    }

    vector::Color3 *Image::operator[](size_t index) {
        return &buffer_[width_ * index];
    }

    void Image::save_ppm(const std::string &output_file) {
        std::ofstream output_stream(output_file);
        fmt::print(output_stream, "P3\n{} {}\n{}\n", width_, height_, static_cast<int>(MAX_COLOR_VALUE));
        for (const auto &color : buffer_) {
            auto casted_image = color.cast<uint8_t>(MAX_COLOR_VALUE);
            fmt::print(output_stream, "{} {} {}\n", casted_image[0], casted_image[1], casted_image[2]);
        }
    }

    void Image::save_png(const std::string &output_file) {
        std::vector<uint8_t> png_image_buffer(height_ * width_ * 3);
        size_t current_buffer_i = 0;
        for (const auto &pixel : buffer_) {
            auto casted_image = pixel.cast<uint8_t>(MAX_COLOR_VALUE);
            png_image_buffer[current_buffer_i++] = casted_image[0];
            png_image_buffer[current_buffer_i++] = casted_image[1];
            png_image_buffer[current_buffer_i++] = casted_image[2];
        }

        stbi_write_png(output_file.c_str(), width_, height_, 3, png_image_buffer.data(), width_ * 3);
    }

    void
    Image::save_image(const std::string &output_file, OutputFormat output_format) {
        switch (output_format) {
            case PPM:
                save_ppm(output_file);
                break;
            case PNG:
                save_png(output_file);
                break;
        }
    }

}
