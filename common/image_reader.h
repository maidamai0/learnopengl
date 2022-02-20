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

#include <filesystem>
#include <string>

#include "common/log.h"
#include "glad/glad.h"
#include "stb_image.h"

class ImageReader final {
 public:
  explicit ImageReader(std::string&& image_path) {
    stbi_set_flip_vertically_on_load(1);
    raw_image_data_ = stbi_load(image_path.c_str(), &widht_, &height_, &num_of_color_channels_, 0);
    if (!raw_image_data_) {
      LOGE("load image:{} failed:{}", image_path, stbi_failure_reason());
      throw std::runtime_error("load image failed");
    }

    std::filesystem::path path_info(image_path);
    if (path_info.extension().generic_string() == std::string(".png")) {
      switch (num_of_color_channels_) {
        case 1:
          image_type_ = GL_RED;
          break;
        case 3:
          image_type_ = GL_RGB;
          break;
        case 4:
          image_type_ = GL_RGBA;
          break;
        default:
          LOGI("unkonwn image type:{}", num_of_color_channels_);
          abort();
          break;
      }
    }

    LOGI("image[{}], width:{}, height:{}, channels:{}",
         std::filesystem::absolute(path_info).generic_string(), widht_, height_,
         num_of_color_channels_);

    // TODO
    // test if destructor is called when exception is throwed here.
  }

  ~ImageReader() {
    assert(raw_image_data_);
    stbi_image_free(raw_image_data_);
  }

  [[nodiscard]] auto GetRawData() const -> const unsigned char* {
    return raw_image_data_;
  }

  [[nodiscard]] auto GetWidth() const {
    return widht_;
  }

  [[nodiscard]] auto GetHeight() const {
    return height_;
  }

  [[nodiscard]] auto GetColorChannelNum() const {
    return num_of_color_channels_;
  }

  [[nodiscard]] auto GetImageType() const {
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