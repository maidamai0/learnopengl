/**
 * @file main.cpp
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include "common/glfw_helpper.h"

/*              |
                x0.5
            *   x   x
   -0.5   *     x       x
----x  x  x  x  x  x  x  x--------
        x       x       *   0.5
            x   x   *
                x -0.5
                |
 */

// clang-format off
static const float vertices[] =
{
    0.0F, 0.5F, 0.0F,
    0.5F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F,


    -0.5F, 0.0F, 0.0F,
    0.0F, 0.0F, 0.0F,
    0.0F, -0.5F, 0.0F,
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto pWd = glfwCreateWindow(640, 480, "hello, triangle", nullptr, nullptr);
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
    if (!gladLoadGL()) {
        fmt::print("Load OpenGL failed!\n");
        return -1;
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // create shader
    const GLchar *vertex_shader_source = nullptr;
    const char *fragment_shader_source = nullptr;

    std::string tmp = read_shader("vs.glsl");
    vertex_shader_source = tmp.c_str();

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    if (!check_compile(vertex_shader, vertex_shader_source)) {
        return -1;
    }

    tmp = read_shader("fs.glsl");
    fragment_shader_source = tmp.c_str();
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    if (!check_compile(fragment_shader, fragment_shader_source)) {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell opengl how to interpret our data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // unbind vao
    glBindVertexArray(0);

    // set viewport
    int height = 0;
    int width = 0;
    glfwGetFramebufferSize(pWd, &width, &height);
    glViewport(0, 0, width, height);

    // buffer swapping setting
    glfwSwapInterval(1);

    // draw line instead of fill
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shader_program);
        glBindVertexArray(vao);

        const GLuint len = sizeof(vertices);
        assert(!(len % 3) && "size of vertices must be times of 3");
        const GLuint num = sizeof(vertices) / 3 / sizeof(vertices[0]);
        //        cout << "numer is " << num << "\n";

        glDrawArrays(GL_TRIANGLES, 0, num);

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
