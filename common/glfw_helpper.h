#pragma once

/**
 * @file glfw_wrapper.h
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief wrapper of glfw3
 * @version 0.1
 * @date 2019-08-27
 * 
 */

#include "GLFW/glfw3.h"
#include "fmt/core.h"

#ifdef _MSC_VER
    #ifdef COMMON_DLL_EXPORT
        #define COMMON_API __declspec(dllexport)
    #else
        #define COMMON_API __declspec(dllimport)
    #endif
#endif

#define GLFW_GUARD \
class GlfwGuard { \
public: \
    GlfwGuard() { \
        if(!glfwInit()) { \
            fmt::print(stderr, "glfw initialize failed\n"); \
            exit(-1); \
        } \
    \
        fmt::print(stderr, "glfw initialize succeed\n"); \
    } \
    ~GlfwGuard() { \
        glfwTerminate(); \
        fmt::print(stderr, "glfw terminated\n"); \
    } \
} gurard;

COMMON_API void err_callback(int err, const char *msg);
COMMON_API void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods);
COMMON_API void resize_callback(GLFWwindow *pWd, int w, int h);