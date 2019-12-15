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

#include <cassert>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>

#include "fmt/core.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader {
   public:
    Shader(std::string&& vertex_source_path, std::string&& fragment_source_path) {
        // create vertex shader
        std::string source_tmp = read_shader_source(vertex_source_path);
        const auto vertex_source = source_tmp.c_str();
        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
        glCompileShader(vertex_shader);
        check_compile(vertex_shader, vertex_source);

        // create fragment shader
        source_tmp = read_shader_source(fragment_source_path);
        const auto fragment_source = source_tmp.c_str();
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

        // use
        Use();
    }
    ~Shader() {
        glDeleteProgram(program_);
    }

    auto GetProgram() const {
        return program_;
    }

    void Use() {
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
        glUniformMatrix4fv(get_location_of_uniform(std::move(name)), 1, false, glm::value_ptr(mat));
        check_error();
    }

   private:
    /**
     * @brief read shader file, return shader source in std::string
     *
     * @param shader_file_path  file path
     * @return read_shader      shader source
     */
    std::string read_shader_source(const std::string& shader_file_path) const {
        std::ifstream shader_reader;
        std::stringstream buff;
        shader_reader.open(shader_file_path);
        if (!shader_reader.is_open()) {
            fmt::print(stderr, "path:{} is invalid\n", shader_file_path);
            throw ::std::runtime_error("path invalid");
        }
        buff << shader_reader.rdbuf();

        return buff.str();
    }

    bool check_compile(const GLuint shader, const GLchar* const shader_source) {
        GLint success = 0;
        GLchar msg[512] = {0};

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success != GL_TRUE) {
            glGetShaderInfoLog(shader, sizeof(msg), nullptr, msg);
            fmt::print(stderr, "compile shader failed:{}\nsource:{}\n", msg, shader_source);
            throw std::runtime_error("shader compile error");
        }

        return true;
    }

    bool check_link(const GLuint link) {
        GLint success = 0;
        GLchar msg[512] = {0};

        glGetProgramiv(link, GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            glGetShaderInfoLog(link, sizeof(msg), nullptr, msg);
            fmt::print("compile shader failed:{}\n", msg);
            throw std::runtime_error("shader program link error");
        }

        return true;
    }

    void check_error() const {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            fmt::print("OpenGL operation error: {}\n", err);
            abort();
        }
    }

    GLint get_location_of_uniform(std::string&& name) {
        const auto loc = glGetUniformLocation(program_, name.c_str());
        if (loc == -1) {
            fmt::print("{} is not a valid uniform\n", name);
            abort();
        }
        return loc;
    }

   private:
    GLuint program_{std::numeric_limits<GLuint>::max()};  // invalid value.
};
