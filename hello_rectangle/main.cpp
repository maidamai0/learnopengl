/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief
 * @version 0.1
 * @date 2019-08-28
 *
 */

#include "common/glfw_helpper.h"
#include "common/win_main.h"
#include "fs.glsl.h"
#include "vs.glsl.h"

// clang-format off
// point for rectangle
static const float rect[] =
{
    0.5F, 0.5F, 0.0F,
    0.5F, -0.5F, 0.0F,
    -0.5F, -0.5F, 0.0F,
    -0.5F, 0.5F, 0.0F
};

// index for two triangles
static const unsigned int indices[] =
{
    0,1,3,
    1,2,3
};
// clang-format on

auto main(int argc, char **argv) -> int {
    (void)argc;
    (void)argv;

    // initialize glfw
    GLFW_GUARD;

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    auto *pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (pWd == nullptr) {
        fmt::print("create window failed!\n");
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

    // create shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &glsl::vs, nullptr);
    glCompileShader(vertex_shader);
    if (!check_compile(vertex_shader, glsl::vs)) {
        return -1;
    }

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &glsl::fs, nullptr);
    glCompileShader(fragment_shader);
    if (!check_compile(fragment_shader, glsl::fs)) {
        return -1;
    }

    // create shader program
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    if (!check_link(shader_program)) {
        return -1;
    }

    // delete shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // create vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vbo
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

    // tell opengl how to interpret our data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // create ebo
    GLuint ebo = 0;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // unbind vao
    glBindVertexArray(0);

    // set viewport
    int height = 0;
    int width = 0;
    glfwGetFramebufferSize(pWd, &width, &height);
    glViewport(0, 0, width, height);

    // buffer swapping setting
    glfwSwapInterval(1);

    // set draw type
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // running until exit
    while (glfwWindowShouldClose(pWd) == 0) {
        // clear color
        glClearColor(0.5F, 0.4F, 0.5F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_INT, nullptr);

        // event loop
        glfwPollEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    fmt::print("user request to close this window!\n");

    // delete vertex array object and vertex buffer object
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    // destroy window
    glfwDestroyWindow(pWd);

    // terminate
    glfwTerminate();
}
