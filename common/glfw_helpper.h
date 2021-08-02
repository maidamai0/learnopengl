#pragma once

/**
 * @file glfw_helpper.h
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief helpper functions for glfw
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include <array>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "common/log.h"

#if _WIN32
#include <Windows.h>
#endif

// glad must be included before glfw
// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#define GLFW_GUARD GlfwGuard gurard;

class GlfwGuard {
   public:
    GlfwGuard() {
        if (glfwInit() == 0) {
            LOGE("glfw initialize failed");
            exit(-1);
        }

        LOGI("glfw initialize succeed");
    }
    GlfwGuard(const GlfwGuard &) = delete;
    auto operator=(const GlfwGuard &) -> GlfwGuard & = delete;
    GlfwGuard(GlfwGuard &&) = delete;
    auto operator=(GlfwGuard &&) -> GlfwGuard & = delete;
    ~GlfwGuard() {
        glfwTerminate();
        LOGI("glfw terminated");
    }
};

/**
 * @brief glfw error callback
 *
 * @param err   error code
 * @param msg   error message
 */
void err_callback(int err, const char *msg) {
    LOGE("error:{}[{}]", err, msg);
}

/**
 * @brief   glfw key callback
 */
void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        LOGE("Escape pressed");
        glfwSetWindowShouldClose(window, GLFW_TRUE);  // not work
    }
}

/**
 * @brief glfw resize call back
 */
void resize_callback(GLFWwindow *pWd, int w, int h) {
    int h_old = 0;
    int w_old = 0;
    glfwGetFramebufferSize(pWd, &w_old, &h_old);
    LOGD("resize from {}x{} to {}x{}", w_old, h_old, w, h);
    glViewport(0, 0, w, h);
    glfwSwapBuffers(pWd);
}

/**
 * @brief read shader file, return shader source in std::string
 *
 * @param shader_file_path  file path
 * @return read_shader      shader source
 */
auto read_shader(std::string &&shader_file_path) -> std::string {
    std::ifstream shader_reader;
    std::stringstream buff;
    shader_reader.open(shader_file_path);
    if (!shader_reader.is_open()) {
        auto current_path = std::filesystem::current_path();
        throw ::std::runtime_error("file not exist");
    }
    buff << shader_reader.rdbuf();

    return buff.str();
}

auto check_compile(const GLuint shader, const GLchar *const shader_source) -> bool {
    GLint success = 0;
    std::array<GLchar, 512> msg{};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, sizeof(msg), nullptr, msg.data());
        LOGE("compile shader failed:{}source:{}", msg.data(), shader_source);
        return false;
    }

    return true;
}

auto check_link(const GLuint link) -> bool {
    GLint success = 0;
    std::array<GLchar, 512> msg{};

    glGetProgramiv(link, GL_LINK_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(link, sizeof(msg), nullptr, msg.data());
        LOGE("compile shader failed:{}", msg.data());
        return false;
    }

    return true;
}