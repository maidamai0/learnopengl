/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief
 * @version 0.1
 * @date 2019-08-28
 *
 */

#include "common/glfw_helpper.h"

auto main(int argc, char **argv) -> int {
    (void)argc;
    (void)argv;

    GLFW_GUARD;

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    auto *pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (pWd == nullptr) {
        fmt::print(stderr, "create window failed!\n");
    }

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
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // set viewport
    int height = 0;
    int width = 0;
    glfwGetFramebufferSize(pWd, &width, &height);
    glViewport(0, 0, width, height);

    // buffer swapping setting
    glfwSwapInterval(1);

    glClearColor(0.5F, 0.4F, 0.5F, 1.0F);

    // running until exit
    while (glfwWindowShouldClose(pWd) == 0) {
        // rendering
        glClear(GL_COLOR_BUFFER_BIT);

        // event loop
        glfwPollEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    fmt::print("user request to close this window!\n");

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}
