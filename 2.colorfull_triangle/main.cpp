/**
 * @file main.cpp
 * @author tonghao.yuan (tonghao.yuan@csdental.com)
 * @brief
 * @version 0.1
 * @date 2019-08-27
 *
 */

#include <array>

#include "common/color.h"
#include "common/glfw_helpper.h"
#include "common/log.h"
#include "common/win_main.h"
#include "fs.glsl.h"
#include "vs.glsl.h"

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

auto main(int argc, char **argv) -> int {
  (void)argc;
  (void)argv;

  // clang-format off
    static std::array<float, 9> position =
    {
        -0.5F, -0.5F, 0.0F,
        0.5F, -0.5F, 0.0F,
        0.0F, 0.5F, 0.0F,
    };

    static std::array<float, 9> color =
    {
        1.0F, 1.0F, 0.0F,
        0.0F, 1.0F, 1.0F,
        1.0F, 0.0F, 1.0F,
    };
  // clang-format on

  // initialize glfw
  GLFW_GUARD;

  // set error callback
  glfwSetErrorCallback(err_callback);

  // create a window
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  auto *window = glfwCreateWindow(640, 480, APP_NAME, nullptr, nullptr);
  if (window == nullptr) {
    LOGE("create window failed!");
    return -1;
  }

  // set key callback
  glfwSetKeyCallback(window, key_callback);

  // resize callback
  glfwSetFramebufferSizeCallback(window, resize_callback);

  // make opengl context
  glfwMakeContextCurrent(window);

  // initialize gl
  if (gladLoadGL() == 0) {
    LOGE("Load OpenGL failed!");
    return -1;
  }
  LOGI("OpenGL version:{}", glfwGetVersionString());

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
  GLuint position_buffer = 0;
  glGenBuffers(1, &position_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(position), position.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  GLuint color_buffer = 0;
  glGenBuffers(1, &color_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(color), color.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
  glEnableVertexAttribArray(1);

  // unbind vao
  glBindVertexArray(0);

  // set viewport
  int height = 0;
  int width = 0;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // buffer swapping setting
  glfwSwapInterval(1);

  // draw line instead of fill
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glPointSize(5);

  // running until exit
  while (glfwWindowShouldClose(window) == 0) {
    // clear color
    glClearColor(COLOR(color::floralwhite), 1.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangle
    glUseProgram(shader_program);
    glBindVertexArray(vao);

    const GLuint len = sizeof(position);
    assert(!(len % 3) && "size of vertices must be times of 3");

    glDrawArrays(GL_TRIANGLES, 0, 4);

    // event loop
    glfwWaitEvents();

    // display
    glfwSwapBuffers(window);
  }

  LOGI("user request to close this window!");

  // delete vertex array object and vertex buffer object
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &position_buffer);

  // destroy window
  glfwDestroyWindow(window);

  // terminate
  glfwTerminate();
}