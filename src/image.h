#ifndef RAYTRACERPROJECT_IMAGE_H
#define RAYTRACERPROJECT_IMAGE_H

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>
#include "vec3.h"

namespace ray_tracer {
    enum OutputFormat {
        PPM, PNG
    };

    class Image {
    public:
        Image(size_t height, size_t width) : buffer_{height * width}, height_{height}, width_{width} {
            if (buffer_.empty()) {
                throw std::runtime_error("Trying to initialize image with size of 0");
            }
        }

        vector::Color3 *operator[](size_t index);

        void save_image(const std::string &output_file, OutputFormat output_format);
    private:
        void save_ppm(const std::string &output_file);
        void save_png(const std::string &output_file);

        std::vector<vector::Color3> buffer_;
        size_t height_;
        size_t width_;
    };
}

#endif //RAYTRACERPROJECT_IMAGE_H
