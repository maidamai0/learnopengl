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

#include <limits>
#include <string>

#include "common/image_reader.h"
#include "common/path.h"
#include "fmt/core.h"
#include "glad/glad.h"


class Texture {
   public:
    explicit Texture(std::string&& image_path) {
        glGenTextures(1, &texture_);
        glBindTexture(GL_TEXTURE_2D, texture_);
        check_error();

        // texture parameter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        check_error();

        // read image to memory
        ImageReader image_reader{get_resource_full_path(std::move(image_path))};

        // copy image data to OpenGL
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     image_reader.GetImageType(),
                     image_reader.GetWidth(),
                     image_reader.GetHeight(),
                     0,
                     image_reader.GetImageType(),
                     GL_UNSIGNED_BYTE,
                     image_reader.GetRawData());

        check_error();

        // generate minimap
        glGenerateMipmap(GL_TEXTURE_2D);
        check_error();
    }

    auto GetTextureID() const {
        return texture_;
    }

    void check_error() {
        err_ = glGetError();
        if (err_ != GL_NO_ERROR) {
            fmt::print("OpenGL operation error: {}\n", err_);
            abort();
        }
    }

    ~Texture() {
        glDeleteTextures(1, &texture_);
    }

   private:
    GLuint texture_{std::numeric_limits<GLuint>::max()};  // invalid value
    GLenum err_{GL_NO_ERROR};
};
