/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief 
 * @version 0.1
 * @date 2019-08-27
 * 
 */

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "fmt/core.h"

#include "common/glfw_guard.h"
#include "common/define.h"

/**
 * @brief glfw error callback function
 *
 * @param err	error number
 * @param msg 	description
 */
void err_callback(int err, const char *msg) {
    fmt::print("error:{}[{}]\n", err, msg);
}

static void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        fmt::print("Escape pressed\n");
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void resize_callback(GLFWwindow *pWd, int w, int h) {
    int h_old = 0;
    int w_old = 0;
    glfwGetFramebufferSize(pWd, &w_old, &h_old);
    fmt::print("resize from{}x{} to {}x{}\n", w_old, h_old, w, h);
    glViewport(0, 0, w, h);
}

void process_input(GLFWwindow *pWd) {
    (void)pWd;
    // do nothing
}

/**
 * @brief main function
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);

    if (!glfwInit()) {
        fmt::print("glfw initialize failed\n");
        throw std::exception("glfw initialize failed");
    }

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        fmt::print("create window failed!\n");
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if(!gladLoadGL()){
        fmt::print("Load OpenGL failed!\n");
        exit(-1);
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
        // process input
        process_input(pWd);

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

    glfwTerminate();
}
