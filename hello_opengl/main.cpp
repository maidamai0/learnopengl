/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief
 * @version 0.1
 * @date 2019-08-28
 *
 */

#include <iostream>
#include <ostream>

#include "common/color.h"
#include "common/glfw_helpper.h"
#include "common/win_main.h"
#include "fmt/core.h"

auto main(int argc, char **argv) -> int {
    (void)argc;
    (void)argv;

    GLFW_GUARD;

    // set error callback
    glfwSetErrorCallback(err_callback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create a window
    auto *pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (pWd == nullptr) {
        fmt::print(stderr, "create window failed!\n");
    }
    fmt::print("OpenGL version got:{}\n", glfwGetVersionString());
    std::flush(std::cout);

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if (gladLoadGL() == 0) {
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }

    // set viewport
    int height = 0;
    int width = 0;
    glfwGetFramebufferSize(pWd, &width, &height);
    glViewport(0, 0, width, height);

    // buffer swapping setting
    glfwSwapInterval(1);

    glClearColor(COLOR(color::linen), 1.0F);

    // running until exit
    while (glfwWindowShouldClose(pWd) == 0) {
        // rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // event loop
        glfwWaitEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    fmt::print("user request to close this window!\n");

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}
