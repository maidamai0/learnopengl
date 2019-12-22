#pragma once

/**
 * @file image_reader.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief
 * @version 0.1
 * @date 2019-09-01
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <string>

#include "fmt/core.h"
#include "stb/stb_image.h"

#ifdef _WIN32
// documentation here:
// https://docs.microsoft.com/en-us/cpp/standard-library/filesystem?view=vs-2019
// is not correct
#include <filesystem>  // Microsoft-specific implementation header file name
namespace stdfs = std::filesystem;
#endif

#ifdef __linux__
#include <experimental/filesystem>  // C++-standard header file name
namespace stdfs = std::experimental::filesystem;
#endif

class ImageReader {
   public:
    explicit ImageReader(std::string&& image_path) {
        stbi_set_flip_vertically_on_load(true);
        raw_image_data_ =
            stbi_load(image_path.c_str(), &widht_, &height_, &num_of_color_channels_, 0);
        if (!raw_image_data_) {
            fmt::print(stderr, "load image:{} failed:{}\n", image_path, stbi_failure_reason());
            throw std::runtime_error("load image failed");
        }

        stdfs::path path_info(image_path);
        if (path_info.extension().generic_string() == std::string(".png")) {
            switch (num_of_color_channels_) {
                case 1: image_type_ = GL_RED; break;
                case 3: image_type_ = GL_RGB; break;
                case 4: image_type_ = GL_RGBA; break;
                default:
                    fmt::print("unkonwn image type:{}\n", num_of_color_channels_);
                    abort();
                    break;
            }
        }

        fmt::print("image[{}], width:{}, height:{}, channels:{}\n",
                   stdfs::absolute(path_info).generic_string(),
                   widht_,
                   height_,
                   num_of_color_channels_);

        // TODO
        // test if destructor is called when exception is throwed here.
    }

    ~ImageReader() {
        assert(raw_image_data_);
        stbi_image_free(raw_image_data_);
    }

    const unsigned char* GetRawData() const {
        return raw_image_data_;
    }

    auto GetWidth() const {
        return widht_;
    }

    auto GetHeight() const {
        return height_;
    }

    auto GetColorChannelNum() const {
        return num_of_color_channels_;
    }

    auto GetImageType() const {
        return image_type_;
    }

   private:
    // int is used for stb interface
    int widht_{0};
    int height_{0};
    int num_of_color_channels_{0};
    GLenum image_type_{GL_RGB};
    unsigned char* raw_image_data_{nullptr};
};