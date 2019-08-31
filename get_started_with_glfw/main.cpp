/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief seems an old interface, only works for <= OpenGL3.2, and can not work with CORE_PROFILE
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include "linmath/linmath.h"

#include "common/define.h"
#include "common/glfw_helpper.h"

#include <functional>

// vertex
static const struct {
    float x, y;
    float r, g, b;
} vertices[3] = {
    {-0.6f, -0.4f, 1.f, 0.f, 0.f}, {0.6f, -0.4f, 0.f, 1.f, 0.f}, {0.f, 0.6f, 0.f, 0.f, 1.f}};

// vertex shader
static const char *vertex_shader_text =
    "#version 110\n"
    "uniform mat4 MVP;\n"
    "attribute vec3 vCol;\n"
    "attribute vec2 vPos;\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    color = vCol;\n"
    "}\n";

// fragment shader
static const char *fragment_shader_text =
    "#version 110\n"
    "varying vec3 color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = vec4(color, 1.0);\n"
    "}\n";

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
        return -1;
    }

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto pWd = glfwCreateWindow(640, 480, "hello, opengl", nullptr, nullptr);
    if (!pWd) {
        fmt::print("create window failed!\n");
        return -1;
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
        exit(-1);
    }
    fmt::print("OpenGL version:{}.{}\n", GLVersion.major, GLVersion.minor);

    // buffer swapping setting
    glfwSwapInterval(1);

    // glClearColor(0.5f, 0.4f, 0.5f, 1.0f);

    // vertex buffer
    GLuint vertex_buffer{0};
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertest shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);

    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    GLuint mvp_location = glGetUniformLocation(program, "MVP");
    GLuint vpos_location = glGetAttribLocation(program, "vPos");
    GLuint vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)(0));

    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(
        vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void *)(sizeof(float) * 2));

    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // rendering
        glClear(GL_COLOR_BUFFER_BIT);

        mat4x4 m;
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, static_cast<float>(glfwGetTime()));

        int height = 0, width = 0;
        glfwGetFramebufferSize(pWd, &width, &height);
        float ratio = width / static_cast<float>(height);

        mat4x4 p;
        mat4x4_ortho(p, -ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

        mat4x4 mvp;
        mat4x4_mul(mvp, p, m);

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // event loop
        glfwPollEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    fmt::print("user request to close this window!\n");

    // destroy window
    glfwDestroyWindow(pWd);

    // destroy glfw
    glfwTerminate();
}
