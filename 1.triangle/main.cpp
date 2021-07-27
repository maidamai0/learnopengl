/**
 * @file main.cpp
 * @author tonghao.yuan
 * @brief seems an old interface, only works for <= OpenGL3.2, and can not work with CORE_PROFILE
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include <array>
#include <cstdlib>
#include <functional>

#include "common/define.h"
#include "common/glfw_helpper.h"
#include "common/win_main.h"
#include "dependency/glfw/deps/linmath.h"

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                GLsizei length, const GLchar *message, const void *userParam) {
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type,
            severity,
            message);
}

// clang-format off
const std::array<float, 9> vertices = {
    -0.5F, -0.5F, 0.F,
    0.5F, -0.5F, 0.F,
    0.F, 0.5F, 0.F,
};
// clang-format on

// vertex shader
constexpr auto vertex_shader_text = R"(
#version 410
layout(location = 0) in vec3 vPos;
void main() {
    gl_Position = vec4(vPos, 1.0);
}
)";

// fragment shader
constexpr auto fragment_shader_text = R"(
#version 130
void main() {
    gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

auto main() -> int {
    if (!glfwInit()) {
        return -1;
    }

    // set error callback
    glfwSetErrorCallback(err_callback);

    // create a window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    auto *pWd = glfwCreateWindow(640, 480, APP_NAME, nullptr, nullptr);
    if (!pWd) {
        LOGE("create window failed!");
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
        LOGE("Load OpenGL failed!");
        exit(-1);
    }
    LOGI("OpenGL version:{}.{}", GLVersion.major, GLVersion.minor);

    // buffer swapping setting
    glfwSwapInterval(1);

    glClearColor(0.5f, 0.4f, 0.5f, 1.0f);

    // vertest shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);
    if (!check_compile(vertex_shader, vertex_shader_text)) {
        return EXIT_FAILURE;
    }

    // fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);
    if (!check_compile(fragment_shader, fragment_shader_text)) {
        return EXIT_FAILURE;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    if (!check_link(program)) {
        return EXIT_FAILURE;
    }

    // create vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // vertex buffer
    GLuint vertex_buffer{0};
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    GLuint vpos_location = glGetAttribLocation(program, "vPos");
    assert(vpos_location == 0);

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    glfwMaximizeWindow(pWd);
    // running until exit
    while (!glfwWindowShouldClose(pWd)) {
        // rendering
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // event loop
        glfwWaitEvents();

        // display
        glfwSwapBuffers(pWd);
    }

    LOGI("user request to close this window!");

    // destroy window
    glfwDestroyWindow(pWd);

    // destroy glfw
    glfwTerminate();
}
