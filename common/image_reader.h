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

#include "fmt/core.h"
#include "stb/stb_image.h"

#include <string>

class ImageReader {
   public:
    explicit ImageReader(std::string&& image_path) {
        stbi_set_flip_vertically_on_load(true);
        raw_image_data_ =
            stbi_load(image_path.c_str(), &widht_, &height_, &num_of_color_channels_, 0);
        if (!raw_image_data_) {
            fmt::print(stderr, "load image:{} failed:{}\n", image_path, stbi_failure_reason());
            throw std::exception("load image failed");
        }

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

   private:
    // int is used for stb interface
    int widht_{0};
    int height_{0};
    int num_of_color_channels_{0};
    unsigned char* raw_image_data_{nullptr};
};