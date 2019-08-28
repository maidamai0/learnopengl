/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief 
 * @version 0.1
 * @date 2019-08-28
 * 
 */

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "fmt/core.h"

#include "common/glfw_helpper.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    GLFW_GUARD;

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        fmt::print(stderr, "create window failed!\n");
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if(!gladLoadGL()){
        fmt::print( "Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // set viewport
    int height = 0, width = 0;
    glfwGetFramebufferSize(pWd, &width, &height);
    glViewport(0, 0, width, height);

    // buffer swapping setting
    glfwSwapInterval(1);

    glClearColor(0.5f, 0.4f, 0.5f, 1.0f);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {

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
