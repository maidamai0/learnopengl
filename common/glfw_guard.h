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

class COMMON_API GlfwGuard
{
public:
    GlfwGuard(/* args */);
    ~GlfwGuard();
};
