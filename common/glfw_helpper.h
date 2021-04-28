#pragma once

/**
 * @file glfw_helpper.h
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief helpper functions for glfw
 * @version 0.1
 * @date 2019-08-27
 *
 */
#include "common/common_headers.h"
#include "fmt/core.h"
#include "fmt/format.h"

#if _WIN32
#include <Windows.h>
#endif

// glad must be included before glfw
// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// clang-format on

#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#define GLFW_GUARD                                              \
    class GlfwGuard {                                           \
       public:                                                  \
        GlfwGuard() {                                           \
            if (!glfwInit()) {                                  \
                fmt::print(stderr, "glfw initialize failed\n"); \
                exit(-1);                                       \
            }                                                   \
                                                                \
            fmt::print(stderr, "glfw initialize succeed\n");    \
        }                                                       \
        ~GlfwGuard() {                                          \
            glfwTerminate();                                    \
            fmt::print(stderr, "glfw terminated\n");            \
        }                                                       \
    } gurard;

/**
 * @brief glfw error callback
 *
 * @param err   error code
 * @param msg   error message
 */
void err_callback(int err, const char *msg) {
    fmt::print("error:{}[{}]\n", err, msg);
}

/**
 * @brief   glfw key callback
 */
void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        fmt::print("Escape pressed\n");
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
    fmt::print("resize from{}x{} to {}x{}\n", w_old, h_old, w, h);
    glViewport(0, 0, w, h);
    glfwSwapBuffers(pWd);
}

/**
 * @brief read shader file, return shader source in std::string
 *
 * @param shader_file_path  file path
 * @return read_shader      shader source
 */
std::string read_shader(std::string &&shader_file_path) {
    std::ifstream shader_reader;
    std::stringstream buff;
    shader_reader.open(shader_file_path);
    if (!shader_reader.is_open()) {
        auto current_path = fs::current_path();
        throw ::std::runtime_error("file not exist");
    }
    buff << shader_reader.rdbuf();

    return buff.str();
}

bool check_compile(const GLuint shader, const GLchar *const shader_source) {
    GLint success = 0;
    GLchar msg[512] = {0};

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE) {
        glGetShaderInfoLog(shader, sizeof(msg), nullptr, msg);
        fmt::print(stderr, "compile shader failed:{}\nsource:{}\n", msg, shader_source);
        return false;
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
        return false;
    }

    return true;
}