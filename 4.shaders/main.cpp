/**
 * @file main.cpp
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include <cstdlib>

#include "common/glfw_helpper.h"
#include "common/log.h"
#include "common/shader.h"
#include "common/win_main.h"
#include "shaders_fs.glsl.h"
#include "shaders_vs.glsl.h"

auto main(int argc, char **argv) -> int {
    (void)argc;
    (void)argv;
    // set error callback
    glfwSetErrorCallback(err_callback);

    GLFW_GUARD;

    // create a window
    auto *pWd = glfwCreateWindow(640, 480, APP_NAME, nullptr, nullptr);
    if (!pWd) {
        LOGE("Create window failed!");
        return EXIT_FAILURE;
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // resize callback
    glfwSetFramebufferSizeCallback(pWd, resize_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if (!gladLoadGL()) {
        LOGE("Load OpenGL failed!");
        return EXIT_FAILURE;
    }
    LOGI("OpenGL version:{}.{}", GLVersion.major, GLVersion.minor);

    // get gl info
    LOGI("rederer is {}", glGetString(GL_RENDERER));
    LOGI("version is {}", glGetString(GL_VERSION));

    // config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // triangle point
    // clang-format off
    float vertices[] = {
        -0.5F, -0.5F, 0.0F, 1.0F,0.0F,0.0F,
        0.5F, -0.5F, 0.0F,  0.0F,1.0F,0.0F,
        0.0F, 0.5, 0.0F,    0.0F,0.0F,1.0F
    };
    // clang-format on

    // vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // copy data to graphical card with vbo
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Shader shader(glsl::shaders_vs, glsl::shaders_fs);

    // position
    const auto pos_location = glGetAttribLocation(shader.GetProgram(), "vPos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(pos_location);

    // color
    const auto col_location = glGetAttribLocation(shader.GetProgram(), "vCol");
    glVertexAttribPointer(
        col_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(col_location);

    // clear color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // setup environment
        glBindVertexArray(vao);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // poll event
        glfwWaitEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    LOGE("user request to close this window!");

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}
