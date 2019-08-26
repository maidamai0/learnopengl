/**
 * @file gl.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-04-16
 *
 * @copyright Copyright (c) 2019
 *
 */

// glad
#include "glad/glad.h"

// glfw
#include <GLFW/glfw3.h>

#include <iostream>

using std::cout;
using std::endl;

/**
 * @brief glfw error callback function
 *
 * @param err	error number
 * @param msg 	description
 */
void err_callback(int err, const char *msg) {
    cout << "err:" << err << msg << endl;
}

static void key_callback(GLFWwindow *window, int key, int scan_code, int action, int mods) {
    (void)scan_code;
    (void)mods;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        cout << "Escape pressed" << endl;
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void resize_callback(GLFWwindow *pWd, int w, int h) {
    int h_old = 0;
    int w_old = 0;
    glfwGetFramebufferSize(pWd, &w_old, &h_old);
    cout << "resize from" << w_old << ":" << h_old << " to " << w << ":" << h;
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
    (void)argc;
    (void)argv;

    // initialize glfw
    if (!glfwInit()) {
        cout << "init error, exit" << endl;
        return -1;
    }

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        cout << "create window failed!" << endl;
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if(!gladLoadGL()){
        cout << "Load OpenGL failed!";
        exit(-1);
    }
    cout << "OpenGL version:" << GLVersion.major << "." << GLVersion.minor << endl;

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

    cout << "user request to close this window!" << endl;

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}
