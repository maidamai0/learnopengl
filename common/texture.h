#pragma once

/**
 * @file texture.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief texture wrapper
 * @version 0.1
 * @date 2019-09-01
 *
 * @copyright Copyright (c) 2019
 *
 */

#include "common/image_reader.h"
#include "fmt/core.h"
#include "glad/glad.h"

#include <limits>
#include <string>

class Texture {
   public:
    explicit Texture(std::string&& image_path) {
        glGenTextures(1, &texture_);
        glBindTexture(GL_TEXTURE_2D, texture_);

        // texture parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // read image to memory
        ImageReader image_reader{std::move(image_path)};

        // copy image data to OpenGL
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     image_reader.GetWidth(),
                     image_reader.GetHeight(),
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     image_reader.GetRawData());

        // generate minimap
        glGenerateMipmap(texture_);
    }

    auto GetTextureID() const {
        return texture_;
    }

    ~Texture() {
        glDeleteTextures(1, &texture_);
    }

   private:
    GLuint texture_{std::numeric_limits<GLuint>::max()};  // invalid value
};
