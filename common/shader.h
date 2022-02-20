#pragma once

/**
 * @file shader.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief wrapper of shader program
 * @version 0.1
 * @date 2019-08-31
 *
 * @copyright Copyright (c) 2019
 *
 */

#include <array>
#include <cassert>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>

#include "fmt/core.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader final {
 public:
  Shader(std::string_view vertex_source, std::string_view fragment_source, const GLsizei stride = 0)
      : stride_{stride}, vertex_source_(vertex_source), fragment_source_(fragment_source) {
    init();
    Use();
  }

  ~Shader() {
    glDeleteProgram(program_);
  }

  Shader(const Shader& lhs) = delete;
  auto operator=(const Shader& lhs) -> Shader& = delete;

  Shader(Shader&& lhs) = delete;
  auto operator=(Shader&& lhs) -> Shader& = delete;

  [[nodiscard]] auto GetProgram() const {
    return program_;
  }

  void Use() const {
    glUseProgram(program_);
  }

  auto SetFloat(std::string&& name, const float value) {
    glUniform1f(get_location_of_uniform(std::move(name)), value);
    check_error();
  }

  auto SetVec3(std::string&& name, glm::vec3 vec) {
    glUniform3fv(get_location_of_uniform(std::move(name)), 1, glm::value_ptr(vec));
    check_error();
  }

  auto SetMat4(std::string&& name, glm::mat4 mat) {
    glUniformMatrix4fv(get_location_of_uniform(std::move(name)), 1, 0U, glm::value_ptr(mat));
    check_error();
  }

  auto SetInt(std::string&& name, const int value) {
    glUniform1i(get_location_of_uniform(std::move(name)), value);
    check_error();
  }

  auto SetAttribute(std::string&& name, const GLint size) {
    assert(stride_ > 0 && "set a valid stride before use this functionn");
    const auto loc = get_location_of_attribute(std::move(name));
    glVertexAttribPointer(loc, size, GL_FLOAT, 0U, stride_ * sizeof(float),
                          (void*)(vertex_position_ * sizeof(float)));
    vertex_position_ += size;
    assert(vertex_position_ <= stride_ && "position out of range");
    check_error();
    glEnableVertexAttribArray(loc);
  }

 private:
  auto init() -> void {
    // create vertex shader
    const auto* const vertex_source = vertex_source_.data();
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
    glCompileShader(vertex_shader);
    check_compile(vertex_shader, vertex_source);

    // create fragment shader
    const auto* const fragment_source = fragment_source_.data();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
    glCompileShader(fragment_shader);
    check_compile(fragment_shader, fragment_source);

    // create program
    program_ = glCreateProgram();
    glAttachShader(program_, vertex_shader);
    glAttachShader(program_, fragment_shader);
    glLinkProgram(program_);
    check_link(program_);

    // delete shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
  }

  static auto check_compile(const GLuint shader, const GLchar* const shader_source) -> bool {
    GLint success = 0;
    std::array<GLchar, 512> msg = {0};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
      glGetShaderInfoLog(shader, msg.size(), nullptr, msg.data());
      fmt::print(stderr, "compile shader failed:{}\nsource:{}\n", msg.data(), shader_source);
      throw std::runtime_error("shader compile error");
    }

    return true;
  }

  static auto check_link(const GLuint link) -> bool {
    GLint success = 0;
    std::array<GLchar, 512> msg = {0};

    glGetProgramiv(link, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
      glGetShaderInfoLog(link, msg.size(), nullptr, msg.data());
      fmt::print("compile shader failed:{}\n", msg.data());
      throw std::runtime_error("shader program link error");
    }

    return true;
  }

  void check_error() {
    err_ = glGetError();
    if (err_ != GL_NO_ERROR) {
      fmt::print("WARN:OpenGL operation error: {}\n", err_);
      // abort();
    }
  }

  auto get_location_of_uniform(std::string&& name) const -> GLint {
    const auto loc = glGetUniformLocation(program_, name.c_str());
    if (loc == -1) {
      fmt::print("{} is not a valid uniform\n", name);
      // abort();
    }
    return loc;
  }

  auto get_location_of_attribute(std::string&& name) const -> GLint {
    const auto loc = glGetAttribLocation(program_, name.c_str());
    if (loc == -1) {
      fmt::print("{} is not a valid attribute\n", name);
      abort();
    }
    return loc;
  }

  GLuint program_{std::numeric_limits<GLuint>::max()};  // invalid value.
  const GLsizei stride_{0};
  GLint vertex_position_{0};  // current position of vertex data set by glVertexAttribPointer
  GLenum err_{GL_NO_ERROR};
  std::string_view vertex_source_;
  std::string_view fragment_source_;
};
