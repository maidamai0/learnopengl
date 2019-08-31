/**
 * @file main.cpp
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include "common/glfw_helpper.h"

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    // set error callback
    glfwSetErrorCallback(err_callback);

    GLFW_GUARD;

    // create a window
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        fmt::print("create window failed!\n");
    }

    // set key callback
    glfwSetKeyCallback(pWd, key_callback);

    // make opengl context
    glfwMakeContextCurrent(pWd);

    // initialize gl
    if (!gladLoadGL()) {
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // get gl info
    fmt::print("rederer is {}\n", glGetString(GL_RENDERER));
    fmt::print("version is {}\n", glGetString(GL_VERSION));

    // config
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // triangle point
    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,0.0f,0.0f,
        0.5f, -0.5f, 0.0f,  0.0f,1.0f,0.0f,
        0.0f, 0.5, 0.0f,    0.0f,0.0f,1.0f
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

    // shader
    std::string tmp{read_shader("shaders_vs.glsl")};
    const char *vertex_shader = tmp.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    glCompileShader(vs);
    if (!check_compile(vs, vertex_shader)) {
        return -1;
    }

    tmp = read_shader("shaders_fs.glsl");
    const char *fragment_shader = tmp.c_str();
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    glCompileShader(fs);
    if (!check_compile(fs, fragment_shader)) {
        return -1;
    }

    // gpu shade program
    GLuint shade_proram = glCreateProgram();
    glAttachShader(shade_proram, vs);
    glAttachShader(shade_proram, fs);
    glLinkProgram(shade_proram);
    if (!check_link(shade_proram)) {
        return -1;
    }

    // position
    const auto pos_location = glGetAttribLocation(shade_proram, "vPos");
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(pos_location);

    // color
    const auto col_location = glGetAttribLocation(shade_proram, "vCol");
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
        glUseProgram(shade_proram);
        glBindVertexArray(vao);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // poll event
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
